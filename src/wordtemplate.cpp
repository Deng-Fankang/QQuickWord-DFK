#include "wordtemplate.h"
#include "qfile.h"
#include "qfileinfo.h"
#include "qdir.h"

WordTemplateMgr& WordTemplateMgr::Instance()
{
	static WordTemplateMgr mgr;
	return mgr;
}

WordTemplateMgr::~WordTemplateMgr()
{
	for (auto it = word_template_map.begin(); it != word_template_map.end(); ++it)
	{
		delete (*it);
	}
	word_template_map.clear();
}

void WordTemplateMgr::SetTemplateDir(const QString& dir)
{
	QDir tmp_dir(dir);
	for (QString file : tmp_dir.entryList(QDir::Files))
	{
		QString ab_path = tmp_dir.absoluteFilePath(file);
		RegisterTemplate(ab_path);
	}
}

void WordTemplateMgr::RegisterTemplate(const QString& path)
{
	WordTemplate* word_template = new WordTemplate(path);
	if (word_template->ParseXmlTemplate())
	{
		QFileInfo file_info(path);
		QString doc_id = word_template->doc_indicator->xml_iproperty.doc_id;
		filename2doc_id[file_info.fileName().section('.', 0, 0)] = doc_id;
		word_template_map[doc_id] = word_template;
	}
	else
	{
		delete word_template;
	}
}

QString WordTemplateMgr::GetDocId(const QString& file_name)
{
	QString name = file_name.section('.', 0, 0);
	return filename2doc_id.value(name, "");
}

WordTemplate* WordTemplateMgr::GetWordTemplate(const QString& doc_id)
{
	if(word_template_map.find(doc_id) == word_template_map.end())
	{
		return nullptr;
	}
	return word_template_map[doc_id];
}


WordTemplateMgr::WordTemplateMgr()
{
}

WordTemplate::WordTemplate(QString path)
{
	template_path = path;
	cur_mode = WordTemplateMode::Match_Mode;
	is_match = false;
	is_install = false;
	doc_indicator = new DocIndicator();
}

bool WordTemplate::ParseXmlTemplate()
{
	QDomDocument xdoc;
	QFile file(template_path);
	if (!file.open(QIODevice::ReadOnly))
	{
		return 0;
	}
	QString error_msg;
	if (!xdoc.setContent(&file, &error_msg))
	{
		file.close();
		return 0;
	}
	QDomElement doc_elem = xdoc.documentElement();
	
	// 遍历属性
	QDomNamedNodeMap attrnode_map = doc_elem.attributes();
	QMap<QString, QString> attributes;
	for (int i = 0; i < attrnode_map.count(); i++)
	{
		QDomNode attr = attrnode_map.item(i);
		if (!attr.isNull() && attr.isAttr())
		{
			attributes[attr.nodeName()] = attr.nodeValue();
		}

	}
	doc_indicator->SetAttributes(attributes);
	doc_indicator->xml_iproperty.doc_id = attributes.value("doc_id");
	if (doc_indicator->xml_iproperty.doc_id.isEmpty())
	{
		return false;
	}
	return ParseXmlIndicator(doc_elem, doc_indicator);
}

Indicator* WordTemplate::GetIndicator(const QVector<int>& indicator_ids)
{
	return doc_indicator->GetIndicator(indicator_ids);
}

bool WordTemplate::TryMatchAreaContent(const TitleAreaContent* word_content)
{
	int start_child_idx = 0;
	is_match = doc_indicator->IterMatchAndSetContents(word_content, start_child_idx);
	if (is_match == false)
	{
		ClearMatchContent();
	}
	return is_match;
}

bool WordTemplate::TryInstallContent()
{
	is_install = doc_indicator->IterInstallContents();
	if (is_install == false)
	{
		doc_indicator->ClearInstallContent();
	}
	return is_install;
}


TitleAreaContent* WordTemplate::ContructTitleAreaContent(WordTemplateMode mode)
{
	if (mode == WordTemplateMode::Install_Mode)
	{
		if (is_install)
		{
			return doc_indicator->ContructTitleAreaContent(mode);
		}
		else
		{
			return nullptr;
		}
	}
	else
	{
		if (is_match)
		{
			return doc_indicator->ContructTitleAreaContent(mode);
		}
		else
		{
			return false;
		}
	}
}

void WordTemplate::ClearMatchContent()
{
	doc_indicator->ClearMatchContent();
}

WordTemplate::~WordTemplate()
{
	if (doc_indicator)
	{
		delete doc_indicator;
	}
}

bool WordTemplate::ParseXmlIndicator(QDomElement node_element, NotLeafIndicator* parent_indicator)
{
	assert(parent_indicator->GetIndicatorType() == Indicator::IndicatorType::Not_Leaf_Indicator);
	bool ret = true;
	for (int i = 0; i < node_element.childNodes().size(); i++)
	{
		QDomNode child_node = node_element.childNodes().at(i);
		if (child_node.nodeType() == QDomNode::CommentNode)
		{
			continue;
		}
		QDomElement child_node_element = child_node.toElement();
		Indicator* indicator = nullptr;
		if (child_node_element.tagName() == "TitleIndicator")
		{
			indicator = new TitleIndicator(parent_indicator);
			ret = ret && ParseXmlIndicator(child_node_element, dynamic_cast<TitleIndicator*>(indicator));
		}
		else if (child_node_element.tagName() == "TemplateIndicator")
		{
			indicator = new TemplateIndicator(parent_indicator);
			ret = ret && ParseLeafIndicator(child_node_element, dynamic_cast<TemplateIndicator*>(indicator));
		}
		else if (child_node_element.tagName() == "MutiTemplateIndicator")
		{
			indicator = new MultiTemplateIndicator(parent_indicator);
			ret = ret && ParseLeafIndicator(child_node_element, dynamic_cast<MultiTemplateIndicator*>(indicator));
		}
		else
		{
			;
		}
		if (indicator == nullptr)
		{
			return false;
		}
		// 遍历属性
		QDomNamedNodeMap attrnode_map = child_node_element.attributes();
		QMap<QString, QString> attributes;
		for (int i = 0; i < attrnode_map.count(); i++)
		{
			QDomNode attr = attrnode_map.item(i);
			if (!attr.isNull() && attr.isAttr())
			{
				attributes[attr.nodeName()] = attr.nodeValue();
			}

		}
		indicator->SetAttributes(attributes);
	}
	return ret;
}

bool WordTemplate::ParseLeafIndicator(QDomElement node_element, LeafIndicator* leaf_indicator)
{
	bool ret = true;
	for (int i = 0; i < node_element.childNodes().size(); i++)
	{
		QDomNode child_node = node_element.childNodes().at(i);
		if (child_node.nodeType() == QDomNode::CommentNode)
		{
			continue;
		}
		QDomElement child_node_element = child_node.toElement();
		if (child_node_element.tagName() == "Srcs")
		{
			if (leaf_indicator->GetIndicatorLabel() == Indicator::IndicatorLabel::MultiTemplate_Label)
			{
				ret = ret && ParseInstallSources(child_node_element, dynamic_cast<MultiTemplateIndicator*>(leaf_indicator)->install_sources);
			}
			else if (leaf_indicator->GetIndicatorLabel() == Indicator::IndicatorLabel::Template_Label)
			{
				ret = ret && ParseInstallSource(child_node_element.firstChildElement("Src"), dynamic_cast<TemplateIndicator*>(leaf_indicator)->install_source);
			}
			else
			{
				ret = false;
			}
			
		}
		else if (child_node_element.tagName() == "Format")
		{
			ret = ret && ParseTemplateFormat(child_node_element, leaf_indicator->tformat_col);
		}
	}
	return ret;
}

bool WordTemplate::ParseTemplateFormat(QDomElement node_element, TemplateFormatCollection& tformat_col)
{
	bool ret = true;
	for (int i = 0; i < node_element.childNodes().size(); i++)
	{
		QDomNode child_node = node_element.childNodes().at(i);
		if (child_node.nodeType() == QDomNode::CommentNode && child_node.nodeType() != QDomNode::ElementNode)
		{
			continue;
		}
		QDomElement child_node_element = child_node.toElement();
		TemplateFormat* template_format = nullptr;

		if (child_node_element.tagName() == "TitleTemplate")
		{
			template_format = new TitleTemplate();
			ret = ret && ParseTemplateFormat(child_node_element, dynamic_cast<TitleTemplate*>(template_format)->tformat_col);
		}
		else if (child_node_element.tagName() == "TextTemplate")
		{
			template_format = new TextTemplate();
		}
		else if(child_node_element.tagName() == "ListTemplate")
		{
			template_format = new ListTemplate();
		}
		else if (child_node_element.tagName() == "TableTemplate")
		{
			template_format = new TableTemplate();
		}
		else if (child_node_element.tagName() == "ImageTemplate")
		{
			template_format = new ImageTemplate();
		}
		else
		{
			;
		}
		if (template_format == nullptr)
		{
			return false;
		}
		// 遍历属性
		QDomNamedNodeMap attrnode_map = child_node_element.attributes();
		QMap<QString, QString> attributes;
		for (int i = 0; i < attrnode_map.count(); i++)
		{
			QDomNode attr = attrnode_map.item(i);
			if (!attr.isNull() && attr.isAttr())
			{
				attributes[attr.nodeName()] = attr.nodeValue();
			}
		}
		template_format->SetAttributes(attributes);
		QString node_value = child_node.firstChild().toText().nodeValue();
		template_format->SetNodeValue(node_value);
		tformat_col.template_format_vec.push_back(template_format);
	}
	return ret && tformat_col.template_format_vec.size() > 0;
}

bool WordTemplate::ParseInstallSources(QDomElement node_element, QVector<LeafIndicator::InstallSource>& install_sources)
{
	bool ret = true;
	for (int i = 0; i < node_element.childNodes().size(); i++)
	{
		QDomNode child_node = node_element.childNodes().at(i);
		if (child_node.nodeType() == QDomNode::CommentNode)
		{
			continue;
		}
		QDomElement child_node_element = child_node.toElement();
		if (child_node_element.tagName() == "Src")
		{
			LeafIndicator::InstallSource install_source;
			ret = ret && ParseInstallSource(child_node_element, install_source);

			install_sources.push_back(install_source);
		}
	}
	return ret;
}

bool WordTemplate::ParseInstallSource(QDomElement node_element, LeafIndicator::InstallSource& install_source)
{
	for (int i = 0; i < node_element.childNodes().size(); i++)
	{
		QDomNode child_node = node_element.childNodes().at(i);
		if (child_node.nodeType() == QDomNode::CommentNode && child_node.nodeType() != QDomNode::ElementNode)
		{
			continue;
		}
		QDomElement child_node_element = child_node.toElement();
		if (child_node_element.tagName() == "DocId")
		{
			install_source.doc_id = child_node.firstChild().toText().nodeValue();
		}
		else if (child_node_element.tagName() == "IndicatorId")
		{
			QString indicator_string = child_node.firstChild().toText().nodeValue();
			for (QString idx : indicator_string.split('.'))
			{
				install_source.indicators.push_back(idx.toUInt());
			}
		}
		else if (child_node_element.tagName() == "Rules")
		{
			for (int j = 0; j < child_node_element.childNodes().size(); j++)
			{
				QDomNode grandchild_node = child_node_element.childNodes().at(j);
				if (grandchild_node.nodeType() == QDomNode::CommentNode && grandchild_node.nodeType() != QDomNode::ElementNode)
				{
					continue;
				}

				QDomElement grandchild_node_element = grandchild_node.toElement();
				if (grandchild_node_element.tagName() == "Rule")
				{
					LeafIndicator::InstallSource::Rule rule;
					QStringList srclist = grandchild_node_element.attribute("src_tmp").split('.');
					QStringList dstlist = grandchild_node_element.attribute("dst_tmp").split('.');
					std::for_each(srclist.begin(), srclist.end(), [&](QString cn) {rule.src_tmp.push_back(cn.toInt()); });
					std::for_each(dstlist.begin(), dstlist.end(), [&](QString cn) {rule.dst_tmp.push_back(cn.toInt()); });
					install_source.rules.push_back(rule);
				}
			}
		}
	}
	return (!install_source.doc_id.isEmpty()) && (install_source.indicators.size() > 0) && (install_source.rules.size() > 0);
}



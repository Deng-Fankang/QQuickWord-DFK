#include "wordtemplate.h"
#include "qfile.h"

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

void WordTemplateMgr::RegisterTemplate(const QString& path)
{
	WordTemplate* word_template = new WordTemplate(path);
	if (word_template->ParseXmlTemplate())
	{
		word_template_map[word_template->doc_indicator->xml_iproperty.doc_id] = word_template;
	}
	else
	{
		delete word_template;
	}
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
	cur_mode = Mode::Input_Mode;
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
	if (!xdoc.setContent(&file))
	{
		file.close();
		return 0;
	}
	QDomElement doc_elem = xdoc.documentElement();
	doc_indicator->xml_iproperty.doc_id = doc_elem.attribute("doc_id");
	if (doc_indicator->xml_iproperty.doc_id.isEmpty())
	{
		return false;
	}
	return ParseXmlIndicator(doc_elem, doc_indicator);
}

bool WordTemplate::TryMatchAreaContent(const TitleAreaContent* word_content)
{
	int start_child_idx = 0;
	bool ret = doc_indicator->IterMatchAndSetContents(word_content, start_child_idx);
	if (ret == false)
	{
		ClearMatchContent();
	}
	return ret;
}

TitleAreaContent* WordTemplate::GetAreaContent()
{
	return nullptr;
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
			ret = ret && ParseInstallSources(child_node_element, leaf_indicator->install_sources);
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
		if (child_node.nodeType() == QDomNode::CommentNode)
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
			for (int j = 0; j < child_node_element.childNodes().size(); j++)
			{
				QDomNode grandchild_node = child_node_element.childNodes().at(j);
				if (grandchild_node.nodeType() == QDomNode::CommentNode)
				{
					continue;
				}
				QDomElement grandchild_node_element = grandchild_node.toElement();
				if (grandchild_node_element.tagName() == "DocId")
				{
					install_source.doc_id = grandchild_node_element.nodeValue();
				}
				else if (grandchild_node_element.tagName() == "IndicatorId")
				{
					QString indicator_string = grandchild_node_element.nodeValue();
					for (QString idx : indicator_string.split('.'))
					{
						install_source.indicators.push_back(idx.toUInt());
					}
				}
			}
			ret = ret && (!install_source.doc_id.isEmpty()) && (install_source.indicators.size() > 0);
			install_sources.push_back(install_source);
		}
	}
	return ret;
}

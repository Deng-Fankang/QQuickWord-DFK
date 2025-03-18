#include "indicator.h"
#include "wordtemplate.h"

Indicator::Indicator(NotLeafIndicator* parent_)
{
	parent = parent_;
	if (parent_)
	{
		parent_->indicator_children.push_back(this);
	}
}

bool DocIndicator::IterMatchAndSetContents(const TitleAreaContent* parent_area, int& start_child_idx)
{
	if (start_child_idx >= parent_area->GetContentList().size())
	{
		return false;
	}
	for (auto it = indicator_children.begin(); it != indicator_children.end(); ++it)
	{
		bool ret = (*it)->IterMatchAndSetContents(parent_area, start_child_idx);
		if (ret == false)
		{
			return ret;
		}
	}
	return true;
}


//void DocIndicator::ClearMatchContent()
//{
//	for (auto it = indicator_children.begin(); it != indicator_children.end(); it++)
//	{
//		(*it)->ClearMatchContent();
//	}
//}
//
//void DocIndicator::ClearInstallContent()
//{
//	for (auto it = indicator_children.begin(); it != indicator_children.end(); it++)
//	{
//		(*it)->ClearInstallContent();
//	}
//}

TitleAreaContent* DocIndicator::ContructTitleAreaContent(WordTemplateMode mode)
{
	TitleAreaContent* ret_area = new TitleAreaContent(attributes.value("name"));
	for (auto it = indicator_children.begin(); it != indicator_children.end(); it++)
	{
		if ((*it)->GetIndicatorLabel() == Indicator::IndicatorLabel::Title_Label)
		{
			dynamic_cast<TitleIndicator*>(*it)->ContructTitleAreaContent(ret_area, mode);
		}
		else if ((*it)->GetIndicatorLabel() == Indicator::IndicatorLabel::Template_Label)
		{
			if (mode == WordTemplateMode::Match_Mode)
			{
				dynamic_cast<TemplateIndicator*>(*it)->tobj_col_match.SetAreaContent(ret_area);
			}
			else
			{
				dynamic_cast<TemplateIndicator*>(*it)->tobj_col_install.SetAreaContent(ret_area);
			}
		}
		else if ((*it)->GetIndicatorLabel() == Indicator::IndicatorLabel::MultiTemplate_Label)
		{
			if (mode == WordTemplateMode::Match_Mode)
			{
				for (auto tobj_col : dynamic_cast<MultiTemplateIndicator*>(*it)->tobj_col_match_vec)
				{
					tobj_col.SetAreaContent(ret_area);
				}
			}
			else
			{
				for (auto tobj_col : dynamic_cast<MultiTemplateIndicator*>(*it)->tobj_col_install_vec)
				{
					tobj_col.SetAreaContent(ret_area);
				}
			}
			
		}
	}
	return ret_area;
}


TitleIndicator::TitleIndicator(NotLeafIndicator* parent_)
	:NotLeafIndicator(parent_)
{
}


bool TitleIndicator::IterMatchAndSetContents(const TitleAreaContent* parent_area, int& start_child_idx)
{
	if (start_child_idx >= parent_area->GetContentList().size())
	{
		return false;
	}
	QString property_name = attributes.value("name", "");
	if (property_name == "")
	{
		return false;
	}
	const AreaContent* child_area = parent_area->GetChildContent(start_child_idx);
	if (child_area->GetAreaType() == TITLE_AREA)
	{
		const TitleAreaContent* title_area = dynamic_cast<const TitleAreaContent*>(child_area);
		if (title_area->title == property_name)
		{
			int start_idx = 0;
			auto it = indicator_children.begin();
			for (; it != indicator_children.end(); ++it)
			{
				bool ret = (*it)->IterMatchAndSetContents(title_area, start_idx);
				if (ret == false)
				{
					break;
				}
			}
			if (it == indicator_children.end())
			{
				qDebug() << QString("match TitleIndicator:%1 child_idx:%2\n").arg(title_area->title, QString::number(start_child_idx));
				start_child_idx++;
				return true;
			}
		}

	}
	return false;
}

TitleAreaContent* TitleIndicator::ContructTitleAreaContent(TitleAreaContent* parent_area, WordTemplateMode mode)
{
	assert(parent_area != nullptr);
	TitleAreaContent* ret_area = parent_area->AppendTitleAreaContent(attributes.value("name"), parent_area->title_format);
	for (auto it = indicator_children.begin(); it != indicator_children.end(); it++)
	{
		if ((*it)->GetIndicatorLabel() == Indicator::IndicatorLabel::Title_Label)
		{
			dynamic_cast<TitleIndicator*>(*it)->ContructTitleAreaContent(ret_area, mode);
		}
		else if ((*it)->GetIndicatorLabel() == Indicator::IndicatorLabel::Template_Label)
		{
			if (mode == WordTemplateMode::Match_Mode)
			{
				dynamic_cast<TemplateIndicator*>(*it)->tobj_col_match.SetAreaContent(ret_area);
			}
			else
			{
				dynamic_cast<TemplateIndicator*>(*it)->tobj_col_install.SetAreaContent(ret_area);
			}
		}
		else if ((*it)->GetIndicatorLabel() == Indicator::IndicatorLabel::MultiTemplate_Label)
		{
			if (mode == WordTemplateMode::Match_Mode)
			{
				for (auto tobj_col : dynamic_cast<MultiTemplateIndicator*>(*it)->tobj_col_match_vec)
				{
					tobj_col.SetAreaContent(ret_area);
				}
			}
			else
			{
				for (auto tobj_col : dynamic_cast<MultiTemplateIndicator*>(*it)->tobj_col_install_vec)
				{
					tobj_col.SetAreaContent(ret_area);
				}
			}
		}
	}
	return ret_area;
}

TemplateIndicator::TemplateIndicator(NotLeafIndicator* parent_)
	:LeafIndicator(parent_)
{
	
}

bool TemplateIndicator::IterMatchAndSetContents(const TitleAreaContent* parent_area, int& start_child_idx)
{
	if (start_child_idx >= parent_area->GetContentList().size())
	{
		return false;
	}
	ClearMatchContent();
	int iter_child_idx = start_child_idx;
	tobj_col_match = tformat_col.ProduceObjByIterMatch(parent_area, iter_child_idx);
	if (tobj_col_match.GetObjVec().size() > 0)
	{
		start_child_idx = iter_child_idx;
		return true;
	}
	return false;
}

bool TemplateIndicator::IterInstallContents()
{
	tobj_col_install = tformat_col.ProduceEmptyObj();
	bool ret = true;
	QMap<const TemplateFormat::TObj*, const TemplateFormat::TObj*> template_obj_map;
	WordTemplate* src_word_tmp = WordTemplateMgr::Instance().GetWordTemplate(install_source.doc_id);
	if (src_word_tmp)
	{
		Indicator* src_indicator = src_word_tmp->GetIndicator(install_source.indicators);
		if (src_indicator && src_indicator->GetIndicatorLabel() == Indicator::IndicatorLabel::Template_Label)
		{
			for (InstallSource::Rule rule : install_source.rules)
			{
				const TemplateFormat::TObj* src_tobj = dynamic_cast<TemplateIndicator*>(src_indicator)->tobj_col_match.GetTemplateFormatObj(rule.src_tmp);
				const TemplateFormat::TObj* dst_tobj = tobj_col_install.GetTemplateFormatObj(rule.dst_tmp);
				if (src_tobj && dst_tobj)
				{
					template_obj_map[dst_tobj] = src_tobj;
				}
				else
				{
					return false;
				}
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
	tobj_col_install.InstallObjContent(template_obj_map);
	return true;
}


void TemplateIndicator::ClearMatchContent()
{
	tobj_col_match.ClearObjVec();
}

void TemplateIndicator::ClearInstallContent()
{
	tobj_col_install.ClearObjVec();
}

MultiTemplateIndicator::MultiTemplateIndicator(NotLeafIndicator* parent_)
	:LeafIndicator(parent_)
{

}

bool MultiTemplateIndicator::IterMatchAndSetContents(const TitleAreaContent* parent_area, int& start_child_idx)
{
	if (start_child_idx >= parent_area->GetContentList().size())
	{
		return false;
	}
	ClearMatchContent();
	QString property_stop_title = attributes.value("stop_title", "");
	while (start_child_idx < parent_area->GetContentList().size())
	{
		int iter_child_idx = start_child_idx;
		TemplateFormatObjCollection tobj_col = tformat_col.ProduceObjByIterMatch(parent_area, iter_child_idx);
		if (tobj_col.GetObjVec().size() > 0)
		{
			tobj_col_match_vec.push_back(tobj_col);
			start_child_idx = iter_child_idx;
		}
		else
		{
			break;
		}
		if (iter_child_idx < parent_area->GetContentList().size())
		{
			const AreaContent* area_content = parent_area->GetChildContent(iter_child_idx);
			if (!property_stop_title.isEmpty() && area_content->GetAreaType() == TITLE_AREA)
			{
				const TitleAreaContent* title_area = dynamic_cast<const TitleAreaContent*>(area_content);
				if (title_area->title == property_stop_title)
				{
					break;
				}
			}
		}
	}
	return tobj_col_match_vec.size() > 0;
}

bool MultiTemplateIndicator::IterInstallContents()
{
	for (const InstallSource install_source : install_sources)
	{
		// 安装信息到模板
		WordTemplate* src_word_tmp = WordTemplateMgr::Instance().GetWordTemplate(install_source.doc_id);
		if (src_word_tmp)
		{
			Indicator* src_indicator = src_word_tmp->GetIndicator(install_source.indicators);
			if (src_indicator)
			{
				if (src_indicator->GetIndicatorLabel() == Indicator::IndicatorLabel::Template_Label)
				{
					TemplateFormatObjCollection tobj_col_install = tformat_col.ProduceEmptyObj();
					const TemplateFormatObjCollection& src_tobj_col = dynamic_cast<TemplateIndicator*>(src_indicator)->GetMatchContent();
					QMap<const TemplateFormat::TObj*, const TemplateFormat::TObj*> template_obj_map;
					for (InstallSource::Rule rule : install_source.rules)
					{
						const TemplateFormat::TObj* src_tobj = src_tobj_col.GetTemplateFormatObj(rule.src_tmp);
						const TemplateFormat::TObj* dst_tobj = tobj_col_install.GetTemplateFormatObj(rule.dst_tmp);
						template_obj_map[dst_tobj] = src_tobj;
					}
					tobj_col_install.InstallObjContent(template_obj_map);
					tobj_col_install_vec.push_back(tobj_col_install);
				}
				else if (src_indicator->GetIndicatorLabel() == Indicator::IndicatorLabel::MultiTemplate_Label)
				{
					const QVector<TemplateFormatObjCollection>& src_tobj_col_vec = dynamic_cast<MultiTemplateIndicator*>(src_indicator)->tobj_col_match_vec;
					for (int i = 0; i < src_tobj_col_vec.size(); i++)
					{
						TemplateFormatObjCollection tobj_col_install = tformat_col.ProduceEmptyObj();
						QMap<const TemplateFormat::TObj*, const TemplateFormat::TObj*> template_obj_map;
						for (InstallSource::Rule rule : install_source.rules)
						{
							const TemplateFormat::TObj* src_tobj = src_tobj_col_vec[i].GetTemplateFormatObj(rule.src_tmp);
							const TemplateFormat::TObj* dst_tobj = tobj_col_install.GetTemplateFormatObj(rule.dst_tmp);
							template_obj_map[dst_tobj] = src_tobj;
						}
						tobj_col_install.InstallObjContent(template_obj_map);
						tobj_col_install_vec.push_back(tobj_col_install);
					}
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	return true;
}

void MultiTemplateIndicator::ClearMatchContent()
{
	for (int i = 0; i < tobj_col_match_vec.size(); i++)
	{
		tobj_col_match_vec[i].ClearObjVec();
	}
	tobj_col_match_vec.clear();
}

void MultiTemplateIndicator::ClearInstallContent()
{
	for (int i = 0; i < tobj_col_match_vec.size(); i++)
	{
		tobj_col_install_vec[i].ClearObjVec();
	}
	tobj_col_install_vec.clear();
}

NotLeafIndicator::NotLeafIndicator(NotLeafIndicator* parent_)
	:Indicator(parent_)
{
}

Indicator* NotLeafIndicator::GetIndicator(const QVector<int>& indicator_ids)
{
	NotLeafIndicator* parent = this;
	Indicator* indicator = nullptr;
	int i = 0;
	for (; i < indicator_ids.size(); i++)
	{
		indicator = parent->indicator_children.at(indicator_ids[i] - 1);
		if (indicator->GetIndicatorType() == Indicator::IndicatorType::Not_Leaf_Indicator)
		{
			parent = dynamic_cast<NotLeafIndicator*>(indicator);
		}
	}
	return indicator;
}

bool NotLeafIndicator::IterInstallContents()
{
	for (auto it = indicator_children.begin(); it != indicator_children.end(); ++it)
	{
		bool ret = (*it)->IterInstallContents();
		if (ret == false)
		{
			return ret;
		}
	}
	return true;
}

void NotLeafIndicator::ClearMatchContent()
{
	for (auto it = indicator_children.begin(); it != indicator_children.end(); it++)
	{
		(*it)->ClearMatchContent();
	}
}

void NotLeafIndicator::ClearInstallContent()
{
	for (auto it = indicator_children.begin(); it != indicator_children.end(); it++)
	{
		(*it)->ClearInstallContent();
	}
}


LeafIndicator::LeafIndicator(NotLeafIndicator* parent_)
	:Indicator(parent_)
{
}

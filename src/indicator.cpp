#include "indicator.h"

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

void DocIndicator::ClearMatchContent()
{
	for (auto it = indicator_children.begin(); it != indicator_children.end(); it++)
	{
		(*it)->ClearMatchContent();
	}
}

TitleIndicator::TitleIndicator(NotLeafIndicator* parent_)
	:NotLeafIndicator(parent_)
{
}

void TitleIndicator::ClearMatchContent()
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
	tobj_col = tformat_col.ProduceObjByIterMatch(parent_area, iter_child_idx);
	if (tobj_col.GetObjVec().size() > 0)
	{
		start_child_idx = iter_child_idx;
		return true;
	}
	return false;
}

void TemplateIndicator::ClearMatchContent()
{
	tobj_col.ClearObjVec();
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
			tobj_col_vec.push_back(tobj_col);
			start_child_idx = iter_child_idx;
		}
		else
		{
			break;
		}
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
	return tobj_col_vec.size() > 0;
}

void MultiTemplateIndicator::ClearMatchContent()
{
	for (int i = 0; i < tobj_col_vec.size(); i++)
	{
		tobj_col_vec[i].ClearObjVec();
	}
	tobj_col_vec.clear();
}

NotLeafIndicator::NotLeafIndicator(NotLeafIndicator* parent_)
	:Indicator(parent_)
{
}

LeafIndicator::LeafIndicator(NotLeafIndicator* parent_)
	:Indicator(parent_)
{
}

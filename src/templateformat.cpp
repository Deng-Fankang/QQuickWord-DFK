#include "templateformat.h"

TemplateFormat::TObj* TitleTemplate::ProduceObjByIterMatch(const TitleAreaContent* parent_area, int& start_child_idx)
{
	const AreaContent* match_area = parent_area->GetChildContent(start_child_idx);
	if (match_area->GetAreaType() == TITLE_AREA)
	{
		const TitleAreaContent* match_titile_area = dynamic_cast<const TitleAreaContent*>(match_area);
		QString property_name = attrbutes.value("name", "");
		if (property_name.isEmpty() || match_titile_area->title == property_name)
		{
			int idx = 0;
			TemplateFormatObjCollection tobj_col = tformat_col.ProduceObjByIterMatch(match_titile_area, idx);
			if (tobj_col.template_format_obj_vec.size() > 0)
			{
				TObj* tobj = new TObj();
				tobj->template_format_obj_col = std::move(tobj_col);
				qDebug() << QString("match TitleTemplate:%1----%2\n").arg(match_titile_area->title, start_child_idx);
				start_child_idx++;
				return tobj;
			}
		}
	}
	return nullptr;
}


TemplateFormat::TObj* TableTemplate::ProduceObjByIterMatch(const TitleAreaContent* parent_area, int& start_child_idx)
{
	int iter_child_index = start_child_idx;
	QVector<const TableAreaContent*> tmp_vec;
	int count = attrbutes.value("count", "0").toInt();
	while (iter_child_index < parent_area->GetContentList().size() && parent_area->GetChildContent(iter_child_index)->GetAreaType() == TABLE)
	{
		tmp_vec.push_back(dynamic_cast<const TableAreaContent*>(parent_area->GetChildContent(iter_child_index)));
		iter_child_index++;
	}
	if (count == 0 || (iter_child_index - start_child_idx == count))
	{
		TObj* tobj = new TObj();
		tobj->table_area_content_vec = std::move(tmp_vec);

		qDebug() << QString("match TableTemplate\n");
		start_child_idx = iter_child_index;
		return tobj;
	}
	else
	{
		return nullptr;
	}
}


TemplateFormat::TObj* ListTemplate::ProduceObjByIterMatch(const TitleAreaContent* parent_area, int& start_child_idx)
{
	int iter_child_index = start_child_idx;
	QVector<const ListAreaContent*> tmp_vec;
	int count = attrbutes.value("count", "0").toInt();
	while (iter_child_index < parent_area->GetContentList().size() && parent_area->GetChildContent(iter_child_index)->GetAreaType() == LIST)
	{
		tmp_vec.push_back(dynamic_cast<const ListAreaContent*>(parent_area->GetChildContent(iter_child_index)));
		iter_child_index++;
	}
	if (count == 0 || (iter_child_index - start_child_idx == count))
	{
		TObj* tobj = new TObj();
		tobj->list_area_content_vec = std::move(tmp_vec);
		QString start_txt = tobj->list_area_content_vec[start_child_idx]->list_data[0].text_content.text_data;
		QString end_txt = "";
		if (tobj->list_area_content_vec.size() > 1)
		{
			end_txt = tobj->list_area_content_vec[iter_child_index - 1]->list_data[0].text_content.text_data;
		}
		qDebug() << QString("match ListTemplate:%1----%2\n").arg(start_txt, end_txt);
		start_child_idx = iter_child_index;
		return tobj;
	}
	else
	{
		return nullptr;
	}
}

TemplateFormat::TObj* ImageTemplate::ProduceObjByIterMatch(const TitleAreaContent* parent_area, int& start_child_idx)
{
	int iter_child_index = start_child_idx;
	QVector<const ImageAreaContent*> tmp_vec;
	int count = attrbutes.value("count", "0").toInt();
	while (iter_child_index < parent_area->GetContentList().size() && parent_area->GetChildContent(iter_child_index)->GetAreaType() == IMAGE)
	{
		tmp_vec.push_back(dynamic_cast<const ImageAreaContent*>(parent_area->GetChildContent(iter_child_index)));
		iter_child_index++;
	}
	if (count == 0 || (iter_child_index - start_child_idx == count))
	{
		TObj* tobj = new TObj();
		tobj->image_area_content_vec = std::move(tmp_vec);

		qDebug() << QString("match ImageTemplate %1\n").arg(count);
		start_child_idx = iter_child_index;
		return tobj;
	}
	else
	{
		return nullptr;
	}
}


TemplateFormat::TObj* TextTemplate::ProduceObjByIterMatch(const TitleAreaContent* parent_area, int& start_child_idx)
{
	int iter_child_index = start_child_idx;

	int count = attrbutes.value("count", "0").toInt();
	bool match_title = attrbutes.value("match_title", "0").toInt();
	QVector<const AreaContent*> tmp_vec;
	while ((iter_child_index < parent_area->GetContentList().size()))
	{
		AreaType area_type = parent_area->GetChildContent(iter_child_index)->GetAreaType();
		if (area_type == TEXT || (area_type == TITLE_AREA && match_title))
		{
			tmp_vec.push_back(parent_area->GetChildContent(iter_child_index));
			iter_child_index++;
		}
		else
		{
			break;
		}
	}
	if (count == 0 || (iter_child_index - start_child_idx) == count)
	{
		TObj* tobj = new TObj();
		tobj->text_area_content_vec = std::move(tmp_vec);
		start_child_idx = iter_child_index;
		const AreaContent* start_area = tobj->text_area_content_vec[0];
		const AreaContent* end_area = tobj->text_area_content_vec[tobj->text_area_content_vec.size() - 1];
		QString start_txt, end_txt;
		if (start_area->GetAreaType() == TEXT)
		{
			start_txt = dynamic_cast<const TextAreaContent*>(start_area)->text_data;
		}
		else
		{
			start_txt = dynamic_cast<const TitleAreaContent*>(start_area)->title;
		}
		if (end_area->GetAreaType() == TEXT)
		{
			end_txt = dynamic_cast<const TextAreaContent*>(end_area)->text_data;
		}
		else
		{
			end_txt = dynamic_cast<const TitleAreaContent*>(end_area)->title;
		}

		
		qDebug() << QString("match TextTemplate:%1----%2\n").arg(start_txt, end_txt);
		return tobj;
	}
	else
	{
		return nullptr;
	}

}

TemplateFormatObjCollection TemplateFormatCollection::ProduceObjByIterMatch(const TitleAreaContent* parent_area, int& start_child_idx)
{
	int iter_child_idx = start_child_idx;
	TemplateFormatObjCollection obj_col;
	auto it = template_format_vec.begin();
	for (; it < template_format_vec.end(); it++)
	{
		TemplateFormat::TObj* tobj = (*it)->ProduceObjByIterMatch(parent_area, iter_child_idx);
		if (tobj)
		{
			obj_col.template_format_obj_vec.push_back(tobj);
		}
		else
		{
			break;
		}
	}
	if (it == template_format_vec.end())
	{
		start_child_idx = iter_child_idx;
	}
	return obj_col;
}

void TemplateFormatObjCollection::ClearObjVec()
{
	for(TemplateFormat::TObj* tobj: template_format_obj_vec)
	{
		delete tobj;
	}
	template_format_obj_vec.clear();
}

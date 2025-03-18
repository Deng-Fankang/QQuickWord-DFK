#include "templateformat.h"
#include "sharefunc.h"
#include "wordconfig.h"

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
				TObj* tobj = new TObj(this, true);
				tobj->template_format_obj_col = std::move(tobj_col);
				tobj->title_name = match_titile_area->title;
				tobj->title_format = match_titile_area->title_format;
				qDebug() << QString("match TitleTemplate:%1----%2\n").arg(match_titile_area->title, start_child_idx);
				start_child_idx++;
				return tobj;
			}
		}
	}
	return nullptr;
}

TemplateFormat::TObj* TitleTemplate::ProduceEmptyObj()
{
	TObj* tobj = new TObj(this, false);
	tobj->template_format_obj_col = tformat_col.ProduceEmptyObj();
	return tobj;
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
		TObj* tobj = new TObj(this, true);
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

TemplateFormat::TObj* TableTemplate::ProduceEmptyObj()
{
	TObj* tobj = new TObj(this, false);
	return tobj;
}


TemplateFormat::TObj* ListTemplate::ProduceObjByIterMatch(const TitleAreaContent* parent_area, int& start_child_idx)
{
	int iter_child_index = start_child_idx;
	QVector<ListAreaContent*> tmp_vec;
	int count = attrbutes.value("count", "0").toInt();
	while (iter_child_index < parent_area->GetContentList().size() && parent_area->GetChildContent(iter_child_index)->GetAreaType() == LIST)
	{
		const ListAreaContent* list_area = dynamic_cast<const ListAreaContent*>(parent_area->GetChildContent(iter_child_index));
		tmp_vec.push_back(const_cast<ListAreaContent*>(list_area));
		iter_child_index++;
	}
	if (count == 0 || (iter_child_index - start_child_idx == count))
	{
		TObj* tobj = new TObj(this, true);
		tobj->list_area_content_vec = std::move(tmp_vec);
		QString start_txt = tobj->list_area_content_vec[0]->list_data[0].text_content.text_data;
		QString end_txt = "";
		if (tobj->list_area_content_vec.size() > 1)
		{
			end_txt = tobj->list_area_content_vec[tobj->list_area_content_vec.size() - 1]->list_data[0].text_content.text_data;
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

TemplateFormat::TObj* ListTemplate::ProduceEmptyObj()
{
	TObj* tobj = new TObj(this, false);
	return tobj;
}

TemplateFormat::TObj* ImageTemplate::ProduceObjByIterMatch(const TitleAreaContent* parent_area, int& start_child_idx)
{
	int iter_child_index = start_child_idx;
	QVector<ImageAreaContent*> tmp_vec;
	int count = attrbutes.value("count", "0").toInt();
	while (iter_child_index < parent_area->GetContentList().size() && parent_area->GetChildContent(iter_child_index)->GetAreaType() == IMAGE)
	{
		const ImageAreaContent* image_area = dynamic_cast<const ImageAreaContent*>(parent_area->GetChildContent(iter_child_index));
		tmp_vec.push_back(const_cast<ImageAreaContent*>(image_area));
		iter_child_index++;
	}
	if (count == 0 || (iter_child_index - start_child_idx == count))
	{
		TObj* tobj = new TObj(this, true);
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

TemplateFormat::TObj* ImageTemplate::ProduceEmptyObj()
{
	TObj* tobj = new TObj(this, false);
	return tobj;
}


TemplateFormat::TObj* TextTemplate::ProduceObjByIterMatch(const TitleAreaContent* parent_area, int& start_child_idx)
{
	int iter_child_index = start_child_idx;

	int count = attrbutes.value("count", "0").toInt();
	bool match_title = attrbutes.value("match_title", "0").toInt();
	QVector<AreaContent*> tmp_vec;
	while ((iter_child_index < parent_area->GetContentList().size()))
	{
		AreaType area_type = parent_area->GetChildContent(iter_child_index)->GetAreaType();
		if (area_type == TEXT || (area_type == TITLE_AREA && match_title))
		{
			tmp_vec.push_back(const_cast<AreaContent*>(parent_area->GetChildContent(iter_child_index)));
			iter_child_index++;
		}
		else
		{
			break;
		}
	}
	if (count == 0 || (iter_child_index - start_child_idx) == count)
	{
		TObj* tobj = new TObj(this, true);
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

TemplateFormat::TObj* TextTemplate::ProduceEmptyObj()
{
	TObj* tobj = new TObj(this, false);
	return tobj;
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

TemplateFormatObjCollection TemplateFormatCollection::ProduceEmptyObj()
{
	TemplateFormatObjCollection obj_col;
	for (auto it = template_format_vec.begin(); it != template_format_vec.end(); it++)
	{
		TemplateFormat::TObj* tobj = (*it)->ProduceEmptyObj();
		obj_col.template_format_obj_vec.push_back(tobj);
	}
	return obj_col;
}

const TemplateFormat::TObj* TemplateFormatObjCollection::GetTemplateFormatObj(const QVector<int>& tformat_ids) const
{
	const TemplateFormatObjCollection* tobj_col = this;
	TemplateFormat::TObj* tobj = nullptr;
	int i = 0;
	for (; i < tformat_ids.size(); i++)
	{
		tobj = tobj_col->template_format_obj_vec.at(tformat_ids[i] - 1);
		if (tobj->GetOwner()->GetTemplateType() == TemplateFormat::TemplateType::Title_Template)
		{
			tobj_col = &(dynamic_cast<TitleTemplate::TObj*>(tobj)->GetTemplateFormatObjCollection());
		}
	}
	return tobj;
}

void TemplateFormatObjCollection::InstallObjContent(const QMap<const TemplateFormat::TObj*, const TemplateFormat::TObj*>& tobj_map)
{
	for (TemplateFormat::TObj* dst_obj : template_format_obj_vec)
	{
		dst_obj->InstallObjContent(tobj_map);
	}
}

void TemplateFormatObjCollection::ClearObjVec()
{
	for(TemplateFormat::TObj* tobj: template_format_obj_vec)
	{
		tobj->ClearTObj();
	}
}

void TemplateFormatObjCollection::SetAreaContent(TitleAreaContent* parent_area)
{
	for (auto it = template_format_obj_vec.begin(); it != template_format_obj_vec.end(); it++)
	{
		(*it)->SetAreaContent(parent_area);
	}
}

TemplateFormat::TObj::TObj(TemplateFormat* owner_, bool ref_out_obj_)
{
	owner = owner_;
	ref_out_obj = ref_out_obj_;
}

void TemplateFormat::TObj::InstallObjContent(const QMap<const TemplateFormat::TObj*, const TemplateFormat::TObj*>& tobj_map)
{
	assert(ref_out_obj == false);
	ClearTObj();
	DoInstallObjContent(tobj_map);
}


void TitleTemplate::TObj::SetAreaContent(TitleAreaContent* parent_area)
{
	TitleAreaContent* title_area = parent_area->AppendTitleAreaContent(title_name, title_format);
	template_format_obj_col.SetAreaContent(title_area);
}

void TitleTemplate::TObj::ClearTObj()
{
	template_format_obj_col.ClearObjVec();
}


void TitleTemplate::TObj::DoInstallObjContent(const QMap<const TemplateFormat::TObj*, const TemplateFormat::TObj*>& tobj_map)
{
	if (tobj_map.find(this) != tobj_map.end())
	{
		if (tobj_map[this]->GetOwner()->GetTemplateType() == TemplateFormat::TemplateType::Title_Template)
		{
			title_name = dynamic_cast<const TitleTemplate::TObj*>(tobj_map[this])->title_name;
			title_format = dynamic_cast<const TitleTemplate::TObj*>(tobj_map[this])->title_format;
		}
		else
		{
			qDebug() << "Title install error:" << endl;
		}
	}
	else
	{
		title_name = owner->GetAttribute("name");
	}
	template_format_obj_col.InstallObjContent(tobj_map);
}


void TableTemplate::TObj::ClearTObj()
{
	if (!ref_out_obj)
	{
		for (int i = 0; i < table_area_content_vec.size(); i++)
		{
			delete table_area_content_vec[i];
		}
	}
	table_area_content_vec.clear();
}

void TableTemplate::TObj::DoInstallObjContent(const QMap<const TemplateFormat::TObj*, const TemplateFormat::TObj*>& tobj_map)
{
	if (tobj_map.find(this) != tobj_map.end())
	{
		if (tobj_map[this]->GetOwner()->GetTemplateType() == TemplateFormat::TemplateType::Table_Template)
		{
			const QVector<const TableAreaContent*>& src_table_area_content_vec = dynamic_cast<const TableTemplate::TObj*>(tobj_map[this])->table_area_content_vec;
			for (int i = 0; i < src_table_area_content_vec.size(); i++)
			{
				table_area_content_vec.push_back(new TableAreaContent(*src_table_area_content_vec[i]));
			}
		}
		else
		{
			qDebug() << "Table install error:" << endl;
		}
	}
	else
	{
		int count = owner->GetAttribute("count", "0").toInt();
		table_area_content_vec.resize(count);
		for (int i = 0; i < count; i++)
		{
			int row = owner->GetAttribute("row", "0").toInt();
			int col = owner->GetAttribute("col", "0").toInt();
			QVector<QVector<QString>> data(row, QVector<QString>(col, ""));
			table_area_content_vec[i] = new TableAreaContent(data);
		}
	}
}

void TableTemplate::TObj::SetAreaContent(TitleAreaContent* parent_area)
{
	for (auto it = table_area_content_vec.begin(); it != table_area_content_vec.end(); it++)
	{
		parent_area->InsertAreaContent(const_cast<TableAreaContent*>(*it));
	}
}


void ListTemplate::TObj::ClearTObj()
{
	if (!ref_out_obj)
	{
		for (int i = 0; i < list_area_content_vec.size(); i++)
		{
			delete list_area_content_vec[i];
		}
	}
	list_area_content_vec.clear();
}

void ListTemplate::TObj::DoInstallObjContent(const QMap<const TemplateFormat::TObj*, const TemplateFormat::TObj*>& tobj_map)
{
	if (tobj_map.find(this) != tobj_map.end())
	{
		if (tobj_map[this]->GetOwner()->GetTemplateType() == TemplateFormat::TemplateType::List_Template)
		{
			const QVector<ListAreaContent*>& src_list_area_content_vec = dynamic_cast<const ListTemplate::TObj*>(tobj_map[this])->list_area_content_vec;
			for (int i = 0; i < src_list_area_content_vec.size(); i++)
			{
				list_area_content_vec.push_back(new ListAreaContent(*src_list_area_content_vec[i]));
			}
		}
		else
		{
			qDebug() << "List install error:" << endl;
		}
	}
	else
	{
		int count = owner->GetAttribute("count", "0").toInt();
		int item_count = owner->GetAttribute("item_count", "1").toInt();
		item_count = qMax(item_count, 1);
		list_area_content_vec.resize(count);
		for (int i = 0; i < count; i++)
		{
			list_area_content_vec[i] = new ListAreaContent(ListTemplateFormat());
			for (int j = 0; j < item_count; j++)
			{
				list_area_content_vec[i]->AppendListItem(QString("Çåµ¥%1").arg(j+1), ListFormat(), TextFormat());
			}
		}
	}
}

void ListTemplate::TObj::SetAreaContent(TitleAreaContent* parent_area)
{
	for (auto it = list_area_content_vec.begin(); it != list_area_content_vec.end(); it++)
	{
		parent_area->InsertAreaContent(const_cast<ListAreaContent*>(*it));
	}
}


void ImageTemplate::TObj::ClearTObj()
{
	if (!ref_out_obj)
	{
		for (int i = 0; i < image_area_content_vec.size(); i++)
		{
			delete image_area_content_vec[i];
		}
	}
	image_area_content_vec.clear();
}

void ImageTemplate::TObj::DoInstallObjContent(const QMap<const TemplateFormat::TObj*, const TemplateFormat::TObj*>& tobj_map)
{
	if (tobj_map.find(this) != tobj_map.end())
	{
		if (tobj_map.value(this)->GetOwner()->GetTemplateType() == TemplateFormat::TemplateType::Image_Template)
		{
			const QVector<ImageAreaContent*>& src_image_area_content_vec = dynamic_cast<const ImageTemplate::TObj*>(tobj_map.value(this))->image_area_content_vec;
			for (int i = 0; i < src_image_area_content_vec.size(); i++)
			{
				image_area_content_vec.push_back(new ImageAreaContent(*src_image_area_content_vec[i]));
			}
		}
		else
		{
			qDebug() << "Image install error:" << endl;
		}
	}
	else
	{
		QString file_path = owner->GetAttribute("url");
		int count = owner->GetAttribute("count", "0").toInt();
		if (!file_path.isEmpty() && count > 0)
		{
			QImage image = QImage(owner->GetAttribute("url"));
			if (!image.isNull())
			{
				for (int i = 0; i < count; i++)
				{
					image_area_content_vec.push_back(new ImageAreaContent(file_path));
				}
				
			}
		}
	}
}

void ImageTemplate::TObj::SetAreaContent(TitleAreaContent* parent_area)
{
	for (auto it = image_area_content_vec.begin(); it != image_area_content_vec.end(); it++)
	{
		parent_area->InsertAreaContent(const_cast<ImageAreaContent*>(*it));
	}
}

void TextTemplate::TObj::ClearTObj()
{
	if (!ref_out_obj)
	{
		for (int i = 0; i < text_area_content_vec.size(); i++)
		{
			delete text_area_content_vec[i];
		}
	}
	text_area_content_vec.clear();
}

void TextTemplate::TObj::DoInstallObjContent(const QMap<const TemplateFormat::TObj*, const TemplateFormat::TObj*>& tobj_map)
{
	if (tobj_map.find(this) != tobj_map.end())
	{
		if (tobj_map.value(this)->GetOwner()->GetTemplateType() == TemplateFormat::TemplateType::Text_Template)
		{
			const QVector<AreaContent*>& src_text_area_content_vec = dynamic_cast<const TextTemplate::TObj*>(tobj_map[this])->text_area_content_vec;
			for (int i = 0; i < src_text_area_content_vec.size(); i++)
			{
				if (src_text_area_content_vec[i]->GetAreaType() == TITLE_AREA)
				{
					const TitleAreaContent* title_area = dynamic_cast<const TitleAreaContent*>(src_text_area_content_vec[i]);
					text_area_content_vec.push_back(TitleAreaContent::CopyTitleContentArea(title_area));
				}
				else
				{
					const TextAreaContent* text_area = dynamic_cast<const TextAreaContent*>(src_text_area_content_vec[i]);
					text_area_content_vec.push_back(new TextAreaContent(*text_area));
				}
			}
		}
		else
		{
			qDebug() << "Text install error:" << endl;
		}
	}
	else
	{
		text_area_content_vec.push_back(new TextAreaContent(owner->GetNodeValue()));
	}
}

void TextTemplate::TObj::SetAreaContent(TitleAreaContent* parent_area)
{
	for (auto it = text_area_content_vec.begin(); it != text_area_content_vec.end(); it++)
	{
		parent_area->InsertAreaContent(const_cast<AreaContent*>(*it));
	}
}
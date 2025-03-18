#include "wordconfig.h"
#include "qjsondocument.h"
#include "qjsonobject.h"
#include "qjsonarray.h"
#include "qfile.h"
#include "qtextcodec.h"

namespace WordConfig
{
	struct
	{
		TextFormat text_format;
	}text_area_format;

	struct
	{
		ListTemplateFormat list_template_format;
		TextFormat text_format;
	}list_area_format;

	struct
	{
		TableFormat table_format;
		TableAreaContent::CellFormat cell_format;
		TextFormat text_format;
	}table_area_format;

	struct
	{
		ImageFormat image_format;
	}image_area_format;

	struct
	{
		ListTemplateFormat list_template_format;
		TextFormat text_format[9];
	}title_area_format;

	void GetTextFormat(const QJsonObject& text_obj, TextFormat& text_format)
	{
		QJsonObject font_obj = text_obj.value("font").toObject();
		QJsonObject paraformat_obj = text_obj.value("paraformat").toObject();
		text_format.font.font_bold = font_obj.value("font_bold").toInt();
		text_format.font.font_italic = font_obj.value("font_italic").toInt();
		text_format.font.font_size = font_obj.value("font_size").toDouble();
		text_format.font.font_spacing = font_obj.value("font_spacing").toDouble();
		text_format.font.font_number_spacing = font_obj.value("font_number_spacing").toDouble();
		text_format.font.font_color = font_obj.value("font_color").toInt();
		text_format.font.font_name = font_obj.value("font_name").toString();
		text_format.font.font_name_fareast = font_obj.value("font_name_fareast").toString();
		text_format.font.font_name_ascii = font_obj.value("font_name_ascii").toString();
		text_format.para_format.line_spacing = paraformat_obj.value("line_spacing").toDouble();
		text_format.para_format.line_spacing_rule = paraformat_obj.value("line_spacing_rule").toInt();
		text_format.para_format.line_unit_after = paraformat_obj.value("line_unit_after").toDouble();
		text_format.para_format.line_unit_before = paraformat_obj.value("line_unit_before").toDouble();
		text_format.para_format.first_line_indent = paraformat_obj.value("first_line_indent").toDouble();
		text_format.para_format.left_indent = paraformat_obj.value("left_indent").toDouble();
		text_format.para_format.right_indent = paraformat_obj.value("right_indent").toDouble();
		text_format.para_format.spacing_after = paraformat_obj.value("spacing_after").toDouble();
		text_format.para_format.spacing_before = paraformat_obj.value("spacing_before").toDouble();
		text_format.para_format.spacing_after_auto = paraformat_obj.value("spacing_after_auto").toDouble();
		text_format.para_format.spacing_before_auto = paraformat_obj.value("spacing_before_auto").toDouble();
		text_format.para_format.alignment = paraformat_obj.value("alignment").toInt();
		text_format.style_name = text_obj.value("style_name").toString();
	}

	void GetListLevelFormat(const QJsonObject& level_obj, ListTemplateFormat::ListLevel& level_format)
	{
		level_format.linked_style = level_obj.value("linked_style").toString();
		level_format.number_format = level_obj.value("number_format").toString();
		level_format.number_pos = level_obj.value("number_pos").toDouble();
		level_format.number_style = level_obj.value("number_style").toInt();
		level_format.text_pos = level_obj.value("text_pos").toDouble();
		level_format.index = level_obj.value("index").toInt();
		level_format.reset_on_higher = level_obj.value("reset_on_higher").toInt();
		level_format.start_at = level_obj.value("start_at").toInt();
		level_format.tab_pos = level_obj.value("tab_pos").toDouble();
		level_format.trailing_character = level_obj.value("trailing_character").toInt();
	}

	void GetBordersFormat(const QJsonObject& borders_obj, BordersFormat& borders_format)
	{
		borders_format.distance_from = borders_obj.value("distance_from").toInt();
		borders_format.distance_from_bottom = borders_obj.value("distance_from_bottom").toInt();
		borders_format.distance_from_left = borders_obj.value("distance_from_left").toInt();
		borders_format.distance_from_right = borders_obj.value("distance_from_right").toInt();
		borders_format.distance_from_top = borders_obj.value("distance_from_top").toInt();
		borders_format.enable = borders_obj.value("enable").toBool();
		borders_format.inside_line_style = borders_obj.value("inside_line_style").toInt();
		borders_format.inside_line_width = borders_obj.value("inside_line_width").toInt();
		borders_format.join_borders = borders_obj.value("join_borders").toBool();
		borders_format.outside_line_style = borders_obj.value("outside_line_style").toInt();
		borders_format.outside_line_width = borders_obj.value("outside_line_width").toInt();

		QJsonArray border_array = borders_obj.value("border_format").toArray();
		for (int i = 0; i < 8; i++)
		{
			QJsonObject border_obj = border_array.at(i).toObject();
			borders_format.border_format[i].color = border_obj.value("color").toInt();
			borders_format.border_format[i].line_style = border_obj.value("line_style").toInt();
			borders_format.border_format[i].line_width = border_obj.value("line_width").toInt();
			borders_format.border_format[i].visible = border_obj.value("visible").toBool();
		}
	}

	void GetTableFormat(const QJsonObject& table_format_obj, TableFormat& table_format, TableAreaContent::CellFormat& cell_format)
	{
		QJsonObject table_obj = table_format_obj.value("table").toObject();
		QJsonObject cell_obj = table_format_obj.value("cell").toObject();
		table_format.allow_auto_fit = table_obj.value("allow_auto_fit").toBool();
		table_format.allow_page_break = table_obj.value("allow_page_break").toBool();
		table_format.apply_style_first_col = table_obj.value("apply_style_first_col").toBool();
		table_format.apply_style_last_col = table_obj.value("apply_style_last_col").toBool();
		table_format.apply_style_heading = table_obj.value("apply_style_heading").toBool();
		table_format.apply_style_last_row = table_obj.value("apply_style_last_row").toBool();
		table_format.table_direction = table_obj.value("table_direction").toInt();
		table_format.auto_format_type = table_obj.value("auto_format_type").toInt();
		table_format.spacing = table_obj.value("spacing").toDouble();
		table_format.top_padding = table_obj.value("top_padding").toDouble();
		table_format.bottom_padding = table_obj.value("bottom_padding").toDouble();
		table_format.left_padding = table_obj.value("left_padding").toDouble();
		table_format.right_padding = table_obj.value("right_padding").toDouble();
		table_format.left_indent = table_obj.value("left_indent").toDouble();
		table_format.alignment = table_obj.value("alignment").toInt();
		table_format.style_name = table_obj.value("style_name").toString();
		GetBordersFormat(table_obj.value("borders_format").toObject(), table_format.borders_format);

		cell_format.height = cell_obj.value("height").toDouble();
		cell_format.width = cell_obj.value("width").toDouble();
		cell_format.height_rule = cell_obj.value("height_rule").toInt();
		cell_format.prefer_width = cell_obj.value("prefer_width").toDouble();
		cell_format.prefer_width_type = cell_obj.value("prefer_width_type").toInt();
		cell_format.top_padding = cell_obj.value("top_padding").toDouble();
		cell_format.bottom_padding = cell_obj.value("bottom_padding").toDouble();
		cell_format.left_padding = cell_obj.value("left_padding").toDouble();
		cell_format.right_padding = cell_obj.value("right_padding").toDouble();
		cell_format.ver_alignment = cell_obj.value("ver_alignment").toInt();
		cell_format.word_wrap = cell_obj.value("word_wrap").toBool();
		cell_format.fit_text = cell_obj.value("fit_text").toBool();
		GetBordersFormat(cell_obj.value("borders_format").toObject(), cell_format.borders_format);
	}
	void GetTextAreaFormat(TextFormat& text_format)
	{
		text_format = text_area_format.text_format;
	}
	void GetTableAreaFormat(TableFormat& table_format, TableAreaContent::CellFormat& cell_format, TextFormat& text_format)
	{
		table_format = table_area_format.table_format;
		text_format = table_area_format.text_format;
		cell_format = table_area_format.cell_format;
	}
	void GetListAreaFormat(ListTemplateFormat& list_template_format, TextFormat& text_format)
	{
		list_template_format = list_area_format.list_template_format;
		text_format = list_area_format.text_format;
	}
	void GetImageAreaFormat(ImageFormat& image_format)
	{
		image_format = image_area_format.image_format;
	}
	void GetTitleAreaFormat(ListTemplateFormat& list_template_format, TextFormat& title_format, int level)
	{
		assert(level >= 1 && level <= 9);
		list_template_format = title_area_format.list_template_format;
		title_format = title_area_format.text_format[level - 1];
	}

	bool ReadTitleFormat()
	{
		QFile title_format_file("config/wordformat/titleformat.json");
		if (title_format_file.open(QIODevice::ReadOnly))
		{
			QJsonParseError json_error;
			QJsonDocument doc = QJsonDocument::fromJson(title_format_file.readAll(), &json_error);
			if (!doc.isObject())
			{
				qDebug() << "titleformat.json Json Parse Error: " << json_error.errorString() << endl;
				title_format_file.close();
				return false;
			}
			QJsonObject obj = doc.object();

			QJsonArray text_array = obj.value("texts").toArray();
			QJsonArray level_array = obj.value("list_levels").toArray();
			int text_array_cnt = text_array.count();
			int level_array_cnt = level_array.count();
			if (text_array_cnt < 1 || level_array_cnt < 1)
			{
				title_format_file.close();
				return false;
			}
			title_area_format.list_template_format.list_type = WdListType(obj.value("list_type").toInt());
			title_area_format.list_template_format.template_id = obj.value("template_id").toInt();
			for (int i = 0; i < 9; i++)
			{
				if (i < text_array_cnt)
				{
					QJsonObject text_obj = text_array.at(i).toObject();
					GetTextFormat(text_obj, title_area_format.text_format[i]);
				}
				else
				{
					title_area_format.text_format[i] = title_area_format.text_format[text_array_cnt - 1];
				}
				if (i < level_array_cnt)
				{
					QJsonObject level_obj = level_array.at(i).toObject();
					GetListLevelFormat(level_obj, title_area_format.list_template_format.list_levels[i]);
				}
				else
				{
					title_area_format.list_template_format.list_levels[i] = title_area_format.list_template_format.list_levels[level_array_cnt - 1];
				}
				title_area_format.list_template_format.list_levels[i].text_font = title_area_format.text_format[i].font;
			}
		}
		else
		{
			title_format_file.close();
			return false;
		}
		title_format_file.close();
		return true;
	}

	bool ReadContentFormat()
	{
		QFile content_format_file("config/wordformat/contentformat.json");
		if (content_format_file.open(QIODevice::ReadOnly))
		{
			QJsonParseError json_error;
			QJsonDocument doc = QJsonDocument::fromJson(content_format_file.readAll(), &json_error);
			if (!doc.isObject())
			{
				qDebug() << "contentformat.json Json Parse Error: " << json_error.errorString() << endl;
				content_format_file.close();
				return false;
			}
			QJsonObject obj = doc.object();

			QJsonObject text_format_obj = obj.value("text_format").toObject();
			QJsonObject list_format_obj = obj.value("list_format").toObject();
			QJsonObject table_format_obj = obj.value("table_format").toObject();
			QJsonObject image_format_obj = obj.value("image_format").toObject();
			
			//text area
			GetTextFormat(text_format_obj, text_area_format.text_format);
			//list area
			GetTextFormat(list_format_obj, list_area_format.text_format);
			list_area_format.list_template_format.list_type = WdListType(list_format_obj.value("list_type").toInt());
			list_area_format.list_template_format.template_id = list_format_obj.value("template_id").toInt();
			QJsonArray level_array = list_format_obj.value("list_levels").toArray();
			int level_cnt = level_array.count();
			if (level_cnt < 1)
			{
				content_format_file.close();
				return false;
			}
			for (int i = 0; i < 9; i++)
			{
				if (i < level_cnt)
				{
					GetListLevelFormat(level_array.at(i).toObject(), list_area_format.list_template_format.list_levels[i]);
				}
				else
				{
					list_area_format.list_template_format.list_levels[i] = list_area_format.list_template_format.list_levels[level_cnt - 1];
				}
			}
			//table area
			GetTextFormat(table_format_obj, table_area_format.text_format);
			GetTableFormat(table_format_obj, table_area_format.table_format, table_area_format.cell_format);
			//image area
			image_area_format.image_format.height = image_format_obj.value("height").toDouble();
			image_area_format.image_format.width = image_format_obj.value("width").toDouble();
			image_area_format.image_format.alignment = image_format_obj.value("alignment").toInt();
			image_area_format.image_format.scale_height = image_format_obj.value("scale_height").toDouble();
			image_area_format.image_format.scale_width = image_format_obj.value("scale_width").toDouble();
		}
		else
		{
			content_format_file.close();
			return false;
		}
		content_format_file.close();
		return true;
	}
	bool InitWordConfig()
	{
		bool ret = ReadTitleFormat();
		if (ret == false)
		{
			return false;
		}
		return ReadContentFormat();
	}
}
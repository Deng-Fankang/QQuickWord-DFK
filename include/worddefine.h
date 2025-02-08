#pragma once
#include "qstring.h"
#include "qvector.h"
#include "qimage.h"
#include "qdebug.h"
#include "qlinkedlist.h"

enum AreaType
{
	TEXT,
	TABLE,
	LIST,
	IMAGE,
	TITLE_AREA,
};


enum WordTitleLevel
{
	RootLevel = 0,
	OneLevel = 1,
	TwoLevel = 2,
	ThreeLevel = 3,
};

enum ExportItemType
{
	TITLE_CONTENT,
	CONTENT,
};

enum WdListType {
	wdListNoNumbering,
	wdListListNumOnly,
	wdListBullet,
	wdListSimpleNumbering,
	wdListOutlineNumbering,
	wdListMixedNumbering,
	wdListPictureBullet,
};

namespace Qt
{
	enum _ItemDataRole
	{
		TitleContentRole = Qt::UserRole,
		AreaContentRole = Qt::UserRole + 1,
	};
};

class TitleContentNode;


class AreaContent
{
public:
	AreaContent(){};
	virtual AreaType GetAreaType() = 0;
	virtual void PrintContent() = 0;
	virtual ~AreaContent() {};
public:
};

struct TextFormat {
	int font_bold;
	int font_italic;
	float font_size;
	float font_spacing;
	float font_number_spacing;

	float line_spacing;
	int line_spacing_rule;
	
	float line_unit_after;
	float line_unit_before;

	float first_line_indent;
	float left_indent;
	float right_indent;

	float spacing_after;
	float spacing_before;

	float spacing_after_auto;
	float spacing_before_auto;
	int alignment;
	QString font_name;
	QString font_name_fareast;
	QString font_name_ascii;
	QString style_name;


	TextFormat& operator=(const TextFormat& cp)
	{
		if (this == &cp)
		{
			return *this;
		}
		font_bold = cp.font_bold;
		font_italic = cp.font_italic;
		font_size = cp.font_size;
		font_spacing = cp.font_spacing;
		font_number_spacing = cp.font_number_spacing;
		line_spacing = cp.line_spacing;
		line_spacing_rule = cp.line_spacing_rule;
		line_unit_after = cp.line_unit_after;
		line_unit_before = cp.line_unit_before;
		first_line_indent = cp.first_line_indent;
		left_indent = cp.left_indent;
		right_indent = cp.right_indent;
		spacing_after = cp.spacing_after;
		spacing_before = cp.spacing_before;
		spacing_after_auto = cp.spacing_after_auto;
		spacing_before_auto = cp.spacing_before_auto;
		alignment = cp.alignment;
		font_name = cp.font_name;
		font_name_fareast = cp.font_name_fareast;
		font_name_ascii = cp.font_name_ascii;
		style_name = cp.style_name;
		return *this;
	}
};

struct ListFormat {
	int list_value;
	int list_level_number;
	QString list_string;
	QString list_style_name;


	ListFormat& operator=(const ListFormat& cp)
	{
		if (this == &cp)
		{
			return *this;
		}
		list_value = cp.list_value;
		list_level_number = cp.list_level_number;
		list_string = cp.list_string;
		list_style_name = cp.list_style_name;
		return *this;
	}
};

struct ListTemplateFormat
{
	QString template_name;
	int list_type;
	struct
	{
		QString number_format;
		float number_pos;
		int number_style;
		float text_pos;
	}list_levels[9];

	ListTemplateFormat& operator=(const ListTemplateFormat& cp)
	{
		if (this == &cp)
		{
			return*this;
		}
		template_name = cp.template_name;
		list_type = cp.list_type;
		for (int i = 0; i < 9; i++)
		{
			list_levels[i].number_format = cp.list_levels[i].number_format;
			list_levels[i].number_pos = cp.list_levels[i].number_pos;
			list_levels[i].number_style = cp.list_levels[i].number_style;
			list_levels[i].text_pos = cp.list_levels[i].text_pos;
		}
		return*this;
	}
};


struct TableFormat
{
	bool allow_auto_fit;
	bool allow_page_break;
	bool apply_style_first_col;
	bool apply_style_last_col;
	bool apply_style_heading;
	bool apply_style_last_row;
	int table_direction;
	int auto_format_type;
	float spacing;
	float top_padding;
	float bottom_padding;
	float left_padding;
	float right_padding;

	float left_indent;
	int alignment;
	QString style_name;
	
	TableFormat& operator=(const TableFormat& cp)
	{
		if (this == &cp)
		{
			return *this;
		}
		allow_auto_fit = cp.allow_auto_fit;
		allow_page_break = cp.allow_page_break;
		apply_style_first_col = cp.apply_style_first_col;
		apply_style_last_col = cp.apply_style_last_col;
		apply_style_heading = cp.apply_style_heading;
		apply_style_last_row = cp.apply_style_last_row;
		table_direction = cp.table_direction;
		auto_format_type = cp.auto_format_type;
		spacing = cp.spacing;
		top_padding = cp.top_padding;
		bottom_padding = cp.bottom_padding;
		left_padding = cp.left_padding;
		right_padding = cp.right_padding;
		left_indent = cp.left_indent;
		alignment = cp.alignment;
		style_name = cp.style_name;
		return *this;
	}
};


struct ImageFormat {
	QString file_name;
	float height;
	float width;
	int alignment;
	float scale_height;
	float scale_width;
	float percent_width;

	ImageFormat& operator=(const ImageFormat& cp)
	{
		if (this == &cp)
		{
			return *this;
		}
		file_name = cp.file_name;
		height = cp.height;
		width = cp.width;
		alignment = cp.alignment;
		scale_height = cp.scale_height;
		scale_width = cp.scale_width;
		percent_width = cp.percent_width;
		return *this;
	}
};


class TextAreaContent :public AreaContent
{
public:

	TextAreaContent(const QString& text = "", const TextFormat& f = {});
	TextAreaContent(const TextAreaContent& cp);

	AreaType GetAreaType() {
		return TEXT;
	}

	void PrintContent() override;

public:
	QString text_data;
	TextFormat text_format;
};

class TableAreaContent :public AreaContent
{
public:
	struct CellFormat
	{
		float height;
		float width;

		int height_rule;

		float prefer_width;
		int prefer_width_type;

		float top_padding;
		float bottom_padding;
		float left_padding;
		float right_padding;

		int ver_alignment;

		bool word_wrap;
		bool fit_text;
	};

	struct CellAreaContent
	{
		QVector<TextAreaContent> cell_data;
		CellFormat cell_format;
	};

	TableAreaContent(const QVector<QVector<CellAreaContent>>& data, const TableFormat& format, QString title = "");
	TableAreaContent(const TableAreaContent& cp);

	AreaType GetAreaType() {
		return TABLE;
	}

	void PrintContent() override;
public:
	QVector<QVector<CellAreaContent>> table_data;
	QString title;
	TableFormat table_format;
};


class ListAreaContent : public AreaContent
{
public:

	struct ListContent
	{
		TextAreaContent text_content;
		ListFormat list_format;
	};

	ListAreaContent(const ListTemplateFormat& list_template_format) 
	{
		this->list_template_format = list_template_format;
	};

	ListAreaContent(const ListAreaContent& cp);

	void AppendListItem(const QString& text, const ListFormat& list_format, const TextFormat& text_format)
	{
		ListContent list_content;
		list_content.text_content.text_data = text;
		list_content.text_content.text_format = text_format;
		list_content.list_format = list_format;
		list_data.push_back(list_content);
	}

	AreaType GetAreaType() {
		return LIST;
	}

	void PrintContent() {

	}

public:
	QVector<ListContent> list_data;
	ListTemplateFormat list_template_format;
};


class ImageAreaContent : public AreaContent
{
public:

	ImageAreaContent(const QImage& img, const ImageFormat& image_format) {
		image = img;
		this->image_format = image_format;
	};

	ImageAreaContent(const ImageAreaContent& cp);

	AreaType GetAreaType() {
		return IMAGE;
	}

	void PrintContent() {

	}

public:
	QImage image;
	ImageFormat image_format;
};


class TitleAreaContent :public AreaContent
{

public:
	TitleAreaContent(TitleContentNode* node_);
	~TitleAreaContent() { if (node) delete node; };

	AreaType GetAreaType() override {
		return TITLE_AREA;
	}

	void PrintContent() override;
public:
	TitleContentNode* node;
};

class TitleContentNode
{
public:

	TitleContentNode(const QString& title_ = "");
	~TitleContentNode();

	AreaContent* AppendTextAreaContent(QString context, TextFormat f);

	AreaContent* AppendTableAreaContent(const QVector<QVector<TableAreaContent::CellAreaContent>>& data, const TableFormat& format, QString area_title = "");

	AreaContent* AppendListAreaContent(const QString& text, const ListTemplateFormat& list_template_format, const ListFormat& list_format, const TextFormat& text_format);

	void AppendListItem(const QString& text, const ListFormat& list_format, const TextFormat& text_format);

	AreaContent* AppendImageAreaContent(const QImage& img, ImageFormat f);

	AreaContent* AppendTitleAreaContent(const QString& title, TextFormat format);

	void InsertAreaContent(AreaContent* content, int insert_idx = -1);

	TitleContentNode* GetParentNode(int level);

	TitleContentNode* GetChildNode(int index);

	const QList<TitleContentNode*>& GetChildTitleList() const { return child_title_list; }
	const QList<AreaContent*>& GetContentList() const{ return content_list; }
	const QVector<int>& GetIndexFromRoot() const { return index_from_root; }
	TitleContentNode* GetParent() { return parent; }
	void setParent(TitleContentNode* parent_) { parent = parent_; }
	int GetTitleLevel() { return level; }

	AreaContent* removeChildTitle(TitleContentNode* node);

	int GetIndex();


private:
	QList<AreaContent*> content_list;
	QList<TitleContentNode*> child_title_list;
	TitleContentNode* parent;
	int level;
	
	QVector<int> index_from_root;
public:
	QString title;
	TextFormat title_format;
};
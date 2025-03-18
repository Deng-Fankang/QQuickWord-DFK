#pragma once
#include "qstring.h"
#include "qvector.h"
#include "qimage.h"
#include "qdebug.h"
#include "qlinkedlist.h"

#define WORD_TMP_PATH "config/wordtemplate"
class TextFormat;
extern const QVector<QString> HEADING_STYLE_NAME;
extern const TextFormat DEFAULT_TEXT_AREA_FORMAT;
extern const TextFormat DEFAULT_TITLE_AREA_FORMAT;

enum class WordTemplateMode
{
	Match_Mode,
	Install_Mode,
};

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

enum WdListType {
	wdListNoNumbering,
	wdListListNumOnly,
	wdListBullet,
	wdListSimpleNumbering,
	wdListOutlineNumbering,
	wdListMixedNumbering,
	wdListPictureBullet,
};


enum WdListGalleryType
{
	wdBulletGallery=1,
	wdNumberGallery,
	wdOutlineNumberGallery,
};


enum WdBorderType
{
	wdBorderDiagonalUp=-8,//从左下角开始的对角线边框。
	wdBorderDiagonalDown=-7,//从左上角开始的对角线边框。
	wdBorderVertical=-6,//纵向框线。
	wdBorderHorizontal=-5,//横向框线。
	wdBorderRight=-4,//右侧框线。
	wdBorderBottom=-3,//底边框线。
	wdBorderLeft=-2,//左侧框线。
	wdBorderTop=-1,//上框线。
};

namespace Qt
{
	enum _ItemDataRole
	{
		TitleContentRole = Qt::UserRole,
		TextContentRole = Qt::UserRole + 1,
		TableContentRole = Qt::UserRole + 2,
		ImageContentRole = Qt::UserRole + 3,
		ListContentRole = Qt::UserRole + 4,
	};
};


struct Font
{
	int font_bold;
	int font_italic;
	float font_size;
	float font_spacing;
	float font_number_spacing;
	int font_color;
	QString font_name;
	QString font_name_fareast;
	QString font_name_ascii;

	Font& operator=(const Font& cp)
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
		font_color = cp.font_color;
		font_name = cp.font_name;
		font_name_fareast = cp.font_name_fareast;
		font_name_ascii = cp.font_name_ascii;
		return *this;
	}
};


struct ParaFormat
{
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

	ParaFormat& operator=(const ParaFormat& cp)
	{
		if (this == &cp)
		{
			return *this;
		}
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
		return *this;
	}
};

struct TextFormat {
	Font font;
	ParaFormat para_format;
	QString style_name;
	
	TextFormat& operator=(const TextFormat& cp)
	{
		if (this == &cp)
		{
			return *this;
		}
		font = cp.font;
		para_format = cp.para_format;
		style_name = cp.style_name;
		return *this;
	}
};

struct ListFormat {
	int list_value;
	int list_level_number;
	QString list_string;
	WdListType list_type;

	ListFormat& operator=(const ListFormat& cp)
	{
		if (this == &cp)
		{
			return *this;
		}
		list_value = cp.list_value;
		list_level_number = cp.list_level_number;
		list_string = cp.list_string;
		list_type = cp.list_type;
		return *this;
	}
};

struct ListTemplateFormat
{
	QString template_name;
	WdListType list_type;
	int template_id;
	bool outline_numberd;
	struct ListLevel
	{
		Font text_font;
		QString linked_style;
		QString number_format;
		float number_pos;
		int number_style;
		float text_pos;
		int index;
		int reset_on_higher;
		int start_at;
		float tab_pos;
		int trailing_character;
	}list_levels[9];

	ListTemplateFormat& operator=(const ListTemplateFormat& cp)
	{
		if (this == &cp)
		{
			return*this;
		}
		template_name = cp.template_name;
		list_type = cp.list_type;
		outline_numberd = cp.outline_numberd;
		for (int i = 0; i < 9; i++)
		{
			list_levels[i].text_font = cp.list_levels[i].text_font;
			list_levels[i].linked_style = cp.list_levels[i].linked_style;
			list_levels[i].number_format = cp.list_levels[i].number_format;
			list_levels[i].number_pos = cp.list_levels[i].number_pos;
			list_levels[i].number_style = cp.list_levels[i].number_style;
			list_levels[i].text_pos = cp.list_levels[i].text_pos;

			list_levels[i].index = cp.list_levels[i].index;
			list_levels[i].reset_on_higher = cp.list_levels[i].reset_on_higher;
			list_levels[i].start_at = cp.list_levels[i].start_at;
			list_levels[i].tab_pos = cp.list_levels[i].tab_pos;
		}
		return*this;
	}
};


struct BordersFormat
{
	int distance_from;
	int distance_from_bottom;
	int distance_from_left;
	int distance_from_right;
	int distance_from_top;
	bool enable;
	int inside_line_style;
	int inside_line_width;
	bool join_borders;
	int outside_line_style;
	int outside_line_width;
	struct
	{
		int art_style;
		int art_width;
		int color;
		int line_style;
		int line_width;
		bool visible;
	}border_format[8];
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
	BordersFormat borders_format;
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
		memcpy(&borders_format, &cp.borders_format, sizeof(borders_format));
		return *this;
	}
};


struct ImageFormat {
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
		height = cp.height;
		width = cp.width;
		alignment = cp.alignment;
		scale_height = cp.scale_height;
		scale_width = cp.scale_width;
		percent_width = cp.percent_width;
		return *this;
	}
};

struct StyleFormat
{
	Font font;
};

class TitleAreaContent;

class AreaContent
{
public:
	AreaContent(TitleAreaContent* parent_area_ = nullptr)
	{
		parent_area = parent_area_;
		is_set_format = false;
	}
	virtual AreaType GetAreaType() const = 0;

	void setParent(TitleAreaContent* parent_area_) { parent_area = parent_area_; }
	TitleAreaContent* GetParent() { return parent_area; }
	bool IsSetFormat() { return is_set_format; }
	virtual void PrintContent() = 0;

	virtual ~AreaContent() { }
protected:
	TitleAreaContent* parent_area;
	bool is_set_format;
};

class TextAreaContent :public AreaContent
{
public:

	TextAreaContent(const QString& text, const TextFormat& f);
	TextAreaContent(const QString& text="");
	TextAreaContent(const TextAreaContent& cp);

	AreaType GetAreaType() const {
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
		BordersFormat borders_format;
	};

	struct CellAreaContent
	{
		QVector<TextAreaContent> cell_data;
		CellFormat cell_format;
	};

	TableAreaContent(const QVector<QVector<CellAreaContent>>& data, const TableFormat& format, QString title = "");
	TableAreaContent(const QVector<QVector<QString>>& data);
	TableAreaContent(const TableAreaContent& cp);

	AreaType GetAreaType() const {
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

	void AppendListItem(const QString& text, const ListFormat& list_format, const TextFormat& text_format);

	AreaType GetAreaType() const {
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

	ImageAreaContent(const QString& image_path, const ImageFormat& image_format);
	ImageAreaContent(const QString& image_path);
	ImageAreaContent(const ImageAreaContent& cp);

	AreaType GetAreaType() const {
		return IMAGE;
	}

	void PrintContent() {

	}

public:
	QString image_path;
	ImageFormat image_format;
};

class TitleAreaContent :public AreaContent
{
public:

	TitleAreaContent(const QString& title_ = "", TitleAreaContent* parent_area_=nullptr);
	TitleAreaContent(const TitleAreaContent& cp);
	~TitleAreaContent();
	AreaType GetAreaType() const override
	{
		return TITLE_AREA;
	}
	void PrintContent() override;

	TextAreaContent* AppendTextAreaContent(QString context, TextFormat f);

	TableAreaContent* AppendTableAreaContent(const QVector<QVector<TableAreaContent::CellAreaContent>>& data, const TableFormat& format, QString area_title = "");

	ListAreaContent* AppendListAreaContent(const QString& text, const ListTemplateFormat& list_template_format, const ListFormat& list_format, const TextFormat& text_format);

	void AppendListItem(const QString& text, const ListFormat& list_format, const TextFormat& text_format);

	ImageAreaContent* AppendImageAreaContent(const QString& img_path, ImageFormat f);

	TitleAreaContent* AppendTitleAreaContent(const QString& title, TextFormat format);

	void InsertAreaContent(AreaContent* content, int insert_idx = -1);

	TitleAreaContent* GetParentAreaLevel(int level);

	TitleAreaContent* GetChildNode(int index);

	const QList<TitleAreaContent*>& GetChildTitleList() const { return child_title_list; }
	const QList<AreaContent*>& GetContentList() const{ return content_list; }
	const AreaContent* GetChildContent(int idx) const;
	const QVector<int>& GetIndexFromRoot() const { return index_from_root; }
	QString GetPrefixIndexString(QChar c) const;

	int GetTitleLevel() const { return level; }

	bool removeAreaContent(AreaContent* content);

	int GetIndex();

	static TitleAreaContent* CopyTitleContentArea(const TitleAreaContent* root, TitleAreaContent* parent=nullptr);
	static void RecurNumberTitleArea(TitleAreaContent* title_area_content, QVector<int> index_from_root = {}, int level = 0);
	static void RecurSetConfigFormat(TitleAreaContent* title_area_content);
private:
	QList<AreaContent*> content_list;
	QList<TitleAreaContent*> child_title_list;
	int level;
	
	QVector<int> index_from_root;
public:
	QString title;
	TextFormat title_format;
	ListTemplateFormat list_template_format;
};

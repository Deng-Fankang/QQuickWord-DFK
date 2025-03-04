#pragma once
#include "qlist.h"
#include "worddefine.h"


class TemplateFormat
{
public:
	class TObj
	{

	};
public:
	enum class TemplateType
	{
		Text_Template,
		Table_Template,
		List_Template,
		Image_Template,
		Title_Template,
	};

	void SetAttributes(const QMap<QString, QString>& attributes_) { attrbutes = attributes_; }
	virtual TemplateType GetTemplateType() = 0;
	//virtual bool IterMatch(const QList<AreaContent*>& area_contents, int& child_idx) = 0;
	virtual TObj* ProduceObjByIterMatch(const TitleAreaContent* parent_area, int& start_child_idx) = 0;
protected:
	QMap<QString, QString> attrbutes;
};


class TemplateFormatObjCollection
{
	friend class TemplateFormatCollection;
	friend class TitleTemplate;
	QVector<TemplateFormat::TObj*> template_format_obj_vec;

public:
	const QVector<TemplateFormat::TObj*>& GetObjVec() { return template_format_obj_vec; }
	void ClearObjVec();
};

class TemplateFormatCollection
{
	friend class TitleTemplate;
	friend class WordTemplate;
public:
	TemplateFormatObjCollection ProduceObjByIterMatch(const TitleAreaContent* parent_area, int& start_child_idx);
private:
	QVector<TemplateFormat*> template_format_vec;
};

class TitleTemplate : public TemplateFormat
{
	friend class WordTemplate;
public:
	class TObj : public TemplateFormat::TObj
	{
		friend class TitleTemplate;
		TemplateFormatObjCollection template_format_obj_col;
		~TObj(){template_format_obj_col.ClearObjVec();}
	};
public:
	TemplateType GetTemplateType() override { return TemplateFormat::TemplateType::Title_Template; }

	TemplateFormat::TObj* ProduceObjByIterMatch(const TitleAreaContent* parent_area, int& start_child_idx);

private:
	TemplateFormatCollection tformat_col;
};

class TableTemplate : public TemplateFormat
{
public:
	class TObj : public TemplateFormat::TObj
	{
		friend class TableTemplate;
		QVector<const TableAreaContent*> table_area_content_vec;
		~TObj() { table_area_content_vec.clear(); }
	};
public:
	TableTemplate() {}
	TemplateType GetTemplateType() override { return TemplateFormat::TemplateType::Table_Template; }
	TemplateFormat::TObj* ProduceObjByIterMatch(const TitleAreaContent* parent_area, int& start_child_idx);
private:

};

class ListTemplate : public TemplateFormat
{
public:
	class TObj: public TemplateFormat::TObj
	{
		friend class ListTemplate;
		QVector<const ListAreaContent*> list_area_content_vec;
		~TObj() { list_area_content_vec.clear(); }
	};
public:
	ListTemplate() {}
	TemplateType GetTemplateType() override { return TemplateFormat::TemplateType::List_Template; }
	TemplateFormat::TObj* ProduceObjByIterMatch(const TitleAreaContent* parent_area, int& start_child_idx);
private:

};


class ImageTemplate : public TemplateFormat
{
public:
	class TObj : public TemplateFormat::TObj
	{
		friend class ImageTemplate;
		QVector<const ImageAreaContent*> image_area_content_vec;

		~TObj() { image_area_content_vec.clear(); }
	};
public:
	ImageTemplate() {}
	TemplateType GetTemplateType() override { return TemplateFormat::TemplateType::Image_Template; }
	TemplateFormat::TObj* ProduceObjByIterMatch(const TitleAreaContent* parent_area, int& start_child_idx);
private:

};


class TextTemplate : public TemplateFormat
{
public:
	class TObj : public TemplateFormat::TObj
	{
		friend class TextTemplate;
		QVector<const AreaContent*> text_area_content_vec;
		~TObj() { text_area_content_vec.clear(); }
	};

public:
	TextTemplate() {  }
	TemplateType GetTemplateType() override { return TemplateFormat::TemplateType::Text_Template; }
	TemplateFormat::TObj* ProduceObjByIterMatch(const TitleAreaContent* parent_area, int& start_child_idx);

private:
	
};


#pragma once
#include "qlist.h"
#include "worddefine.h"


class TemplateFormat
{
public:
	class TObj
	{
	public:
		const TemplateFormat* GetOwner() const { return owner; }
		TObj(TemplateFormat* owner_, bool ref_out_obj_);
		void InstallObjContent(const QMap<const TemplateFormat::TObj*, const TemplateFormat::TObj*>& tobj_map);
		virtual void SetAreaContent(TitleAreaContent* parent_area) = 0;
		virtual void ClearTObj() = 0;
	protected:
		virtual void DoInstallObjContent(const QMap<const TemplateFormat::TObj*, const TemplateFormat::TObj*>& tobj_map) = 0;
		TemplateFormat* owner;
		bool ref_out_obj;
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
	void SetNodeValue(const QString& node_value_) { node_value = node_value_; }

	QString GetAttribute(QString name, QString default_value="") { return attrbutes.value(name, default_value); }
	QString GetNodeValue() { return node_value; }
	virtual TemplateType GetTemplateType() const = 0;
	//virtual bool IterMatch(const QList<AreaContent*>& area_contents, int& child_idx) = 0;
	virtual TObj* ProduceObjByIterMatch(const TitleAreaContent* parent_area, int& start_child_idx) = 0;
	virtual TObj* ProduceEmptyObj() = 0;
protected:
	QMap<QString, QString> attrbutes;
	QString node_value;
};


class TemplateFormatObjCollection
{
	friend class TemplateFormatCollection;
	friend class TitleTemplate;
	QVector<TemplateFormat::TObj*> template_format_obj_vec;

public:
	const QVector<TemplateFormat::TObj*>& GetObjVec() { return template_format_obj_vec; }
	const TemplateFormat::TObj* GetTemplateFormatObj(const QVector<int>& tformat_ids) const;

	void InstallObjContent(const QMap<const TemplateFormat::TObj*, const TemplateFormat::TObj*>& tobj_map);
	void ClearObjVec();

	void SetAreaContent(TitleAreaContent* parent_area);
};

class TemplateFormatCollection
{
	friend class TitleTemplate;
	friend class WordTemplate;
public:
	//void UpdataInstallSource();
	TemplateFormatObjCollection ProduceObjByIterMatch(const TitleAreaContent* parent_area, int& start_child_idx);
	TemplateFormatObjCollection ProduceEmptyObj();
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
	public:
		QString title_name;
		TextFormat title_format;
		
		TObj(TitleTemplate* owner_, bool ref_out_obj) :TemplateFormat::TObj(owner_, ref_out_obj) {}
		const TemplateFormatObjCollection& GetTemplateFormatObjCollection() { return template_format_obj_col; }
		void SetAreaContent(TitleAreaContent* parent_area);
		~TObj(){ ClearTObj();}
		void ClearTObj();
	private:
		void DoInstallObjContent(const QMap<const TemplateFormat::TObj*, const TemplateFormat::TObj*>& tobj_map);
		TemplateFormatObjCollection template_format_obj_col;
		
	};
public:
	TemplateType GetTemplateType() const override { return TemplateFormat::TemplateType::Title_Template; }

	TemplateFormat::TObj* ProduceObjByIterMatch(const TitleAreaContent* parent_area, int& start_child_idx);
	TemplateFormat::TObj* ProduceEmptyObj();
private:
	TemplateFormatCollection tformat_col;
};

class TableTemplate : public TemplateFormat
{
public:
	class TObj : public TemplateFormat::TObj
	{
		friend class TableTemplate;
	public:
		
		TObj(TableTemplate* owner_, bool ref_out_obj) :TemplateFormat::TObj(owner_, ref_out_obj) {}
		void SetAreaContent(TitleAreaContent* parent_area);
		~TObj() { ClearTObj(); }
		void ClearTObj();
	private:
		void DoInstallObjContent(const QMap<const TemplateFormat::TObj*, const TemplateFormat::TObj*>& tobj_map);
		QVector<const TableAreaContent*> table_area_content_vec;
	};
public:
	TableTemplate() {}
	TemplateType GetTemplateType() const override { return TemplateFormat::TemplateType::Table_Template; }
	TemplateFormat::TObj* ProduceObjByIterMatch(const TitleAreaContent* parent_area, int& start_child_idx);
	TemplateFormat::TObj* ProduceEmptyObj();
private:

};

class ListTemplate : public TemplateFormat
{
public:
	class TObj: public TemplateFormat::TObj
	{
		friend class ListTemplate;
		
	public:
		
		TObj(ListTemplate* owner_, bool ref_out_obj) :TemplateFormat::TObj(owner_, ref_out_obj) {}
		void SetAreaContent(TitleAreaContent* parent_area);
		~TObj() { ClearTObj(); }
		void ClearTObj();
	private:
		void DoInstallObjContent(const QMap<const TemplateFormat::TObj*, const TemplateFormat::TObj*>& tobj_map);
		QVector<ListAreaContent*> list_area_content_vec;
	};
public:
	ListTemplate() {}
	TemplateType GetTemplateType() const override { return TemplateFormat::TemplateType::List_Template; }
	TemplateFormat::TObj* ProduceObjByIterMatch(const TitleAreaContent* parent_area, int& start_child_idx);
	TemplateFormat::TObj* ProduceEmptyObj();
private:

};


class ImageTemplate : public TemplateFormat
{
public:
	class TObj : public TemplateFormat::TObj
	{
		friend class ImageTemplate;
		
	public:
		TObj(ImageTemplate* owner_, bool ref_out_obj) :TemplateFormat::TObj(owner_, ref_out_obj) {}
		void SetAreaContent(TitleAreaContent* parent_area);
		~TObj() { ClearTObj(); }
		void ClearTObj();
	private:
		void DoInstallObjContent(const QMap<const TemplateFormat::TObj*, const TemplateFormat::TObj*>& tobj_map);
		QVector<ImageAreaContent*> image_area_content_vec;
	};
public:
	ImageTemplate() {}
	TemplateType GetTemplateType() const override { return TemplateFormat::TemplateType::Image_Template; }
	TemplateFormat::TObj* ProduceObjByIterMatch(const TitleAreaContent* parent_area, int& start_child_idx);
	TemplateFormat::TObj* ProduceEmptyObj();
private:

};


class TextTemplate : public TemplateFormat
{
public:
	class TObj : public TemplateFormat::TObj
	{
		friend class TextTemplate;
	public:
		TObj(TextTemplate* owner_, bool ref_out_obj):TemplateFormat::TObj(owner_, ref_out_obj){}
		void SetAreaContent(TitleAreaContent* parent_area);
		~TObj() { ClearTObj(); }
		void ClearTObj();
	private:
		void DoInstallObjContent(const QMap<const TemplateFormat::TObj*, const TemplateFormat::TObj*>& tobj_map);
		QVector<AreaContent*> text_area_content_vec;
	};

public:
	TextTemplate() {  }
	TemplateType GetTemplateType() const override { return TemplateFormat::TemplateType::Text_Template; }
	TemplateFormat::TObj* ProduceObjByIterMatch(const TitleAreaContent* parent_area, int& start_child_idx);
	TemplateFormat::TObj* ProduceEmptyObj();
private:
	
};


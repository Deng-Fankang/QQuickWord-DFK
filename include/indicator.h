#pragma once
#include "templateformat.h"

class WordTemplate;
class NotLeafIndicator;

class Indicator
{
public:
	enum class IndicatorLabel
	{
		Doc_Label,
		Title_Label,
		Template_Label,
		MultiTemplate_Label,
	};

	enum class IndicatorType
	{
		Leaf_Indicator,
		Not_Leaf_Indicator,
	};

	Indicator(NotLeafIndicator* parent_ = nullptr);
	void SetAttributes(const QMap<QString, QString>& attributes_) { attributes = attributes_; }

	virtual void ClearMatchContent() = 0;
	virtual void ClearInstallContent() = 0;
	virtual IndicatorType GetIndicatorType() = 0;
	virtual IndicatorLabel GetIndicatorLabel() = 0;
	virtual bool IterMatchAndSetContents(const TitleAreaContent* parent_area, int& start_child_idx) = 0;
	virtual bool IterInstallContents() = 0;
protected:
	//WordTemplate* owner;
	NotLeafIndicator* parent;
	QMap<QString, QString> attributes;
};


class NotLeafIndicator : public Indicator
{
	friend class Indicator;
public:
	NotLeafIndicator(NotLeafIndicator* parent_ = nullptr);
	virtual IndicatorType GetIndicatorType() override { return Indicator::IndicatorType::Not_Leaf_Indicator; }
	const QVector<Indicator*>& GetIndicatorChildren() const { return indicator_children; }
	Indicator* GetIndicator(const QVector<int>& indicator_ids);
	bool IterInstallContents();

	void ClearMatchContent();
	void ClearInstallContent();
protected:
	QVector<Indicator*> indicator_children;
};


class LeafIndicator : public Indicator
{
	friend class WordTemplate;
public:
	struct InstallSource
	{
		QString doc_id;
		QVector<int> indicators;
		struct Rule
		{
			QVector<int> src_tmp;
			QVector<int> dst_tmp;
		};
		QVector<Rule> rules;
	};
	LeafIndicator(NotLeafIndicator* parent_ = nullptr);
	virtual IndicatorType GetIndicatorType() override { return Indicator::IndicatorType::Leaf_Indicator; }

protected:
	TemplateFormatCollection tformat_col;
};


class DocIndicator : public NotLeafIndicator
{
public:
	struct XmlIProperty
	{
		QString doc_id;
	};

	IndicatorLabel GetIndicatorLabel() { return IndicatorLabel::Doc_Label; }
	bool IterMatchAndSetContents(const TitleAreaContent* parent_area, int& start_child_idx);
	
	TitleAreaContent* ContructTitleAreaContent(WordTemplateMode mode);
public:
	XmlIProperty xml_iproperty;
};


class TitleIndicator : public NotLeafIndicator
{
public:

	TitleIndicator(NotLeafIndicator* parent_ = nullptr);
	IndicatorLabel GetIndicatorLabel() { return IndicatorLabel::Title_Label; }
	//void ClearMatchContent();
	//void ClearInstallContent();
	bool IterMatchAndSetContents(const TitleAreaContent* parent_area, int& start_child_idx);
	TitleAreaContent* ContructTitleAreaContent(TitleAreaContent* parent_area, WordTemplateMode mode);
public:

};


class TemplateIndicator : public LeafIndicator
{
	friend class WordTemplate;
	friend class DocIndicator;
	friend class TitleIndicator;
public:
	TemplateIndicator(NotLeafIndicator* parent_= nullptr);
	IndicatorLabel GetIndicatorLabel() override { return Indicator::IndicatorLabel::Template_Label; }

	bool IterMatchAndSetContents(const TitleAreaContent* parent_area, int& start_child_idx);
	const TemplateFormatObjCollection& GetMatchContent() { return tobj_col_match; }

	bool IterInstallContents();

	void ClearMatchContent();
	void ClearInstallContent();
private:

private:
	InstallSource install_source;
	TemplateFormatObjCollection tobj_col_match;
	TemplateFormatObjCollection tobj_col_install;
};


class MultiTemplateIndicator : public LeafIndicator
{
	friend class WordTemplate;
	friend class DocIndicator;
	friend class TitleIndicator;
public:
	MultiTemplateIndicator(NotLeafIndicator* parent_ = nullptr);
	IndicatorLabel GetIndicatorLabel() override { return Indicator::IndicatorLabel::MultiTemplate_Label; }

	bool IterMatchAndSetContents(const TitleAreaContent* parent_area, int& start_child_idx);
	bool IterInstallContents();

	void ClearMatchContent();
	void ClearInstallContent();
private:
	QVector<InstallSource> install_sources;
	QVector<TemplateFormatObjCollection> tobj_col_match_vec;
	QVector<TemplateFormatObjCollection> tobj_col_install_vec;
};
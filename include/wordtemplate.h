#pragma once
#include "QtXml/qdom.h"
#include "qstring.h"
#include "indicator.h"
#include "qmap.h"

class WordTemplateMgr;


class WordTemplate
{
	friend class WordTemplateMgr;
public:
	enum class Mode
	{
		Input_Mode,
		Output_Mode,
	};

	WordTemplate(QString path);
	bool ParseXmlTemplate();
	bool TryMatchAreaContent(const TitleAreaContent* word_content);

	TitleAreaContent* GetAreaContent();
	void ClearMatchContent();

	~WordTemplate();

private:
	bool ParseXmlIndicator(QDomElement node_element, NotLeafIndicator* parent_indicator);
	bool ParseLeafIndicator(QDomElement node_element, LeafIndicator* template_indicator);
	bool ParseTemplateFormat(QDomElement node_element, TemplateFormatCollection& tformat_col);
	bool ParseInstallSources(QDomElement node_element, QVector<LeafIndicator::InstallSource>& install_sources);

private:
	Mode cur_mode;
	QString template_path;
	DocIndicator* doc_indicator;
};


class WordTemplateMgr
{
public:
	static WordTemplateMgr& Instance();
	~WordTemplateMgr();
	void RegisterTemplate(const QString& path);
	WordTemplate* GetWordTemplate(const QString& doc_id);
private:
	WordTemplateMgr();

	QMap<QString, WordTemplate*> word_template_map;
};
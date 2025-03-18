#pragma once
#include "QtXml/qdom.h"
#include "qstring.h"
#include "indicator.h"
#include "qmap.h"

class WordTemplateMgr;
class DocIndicator;

class WordTemplate
{
	friend class WordTemplateMgr;
public:

	WordTemplate(QString path);
	bool ParseXmlTemplate();

	Indicator* GetIndicator(const QVector<int>& indicator_ids);
	bool TryMatchAreaContent(const TitleAreaContent* word_content);
	bool TryInstallContent();

	bool IsMatch() { return is_match; }
	bool IsInstall() { return is_install; }

	TitleAreaContent* ContructTitleAreaContent(WordTemplateMode mode);
	void ClearMatchContent();

	~WordTemplate();

private:
	bool ParseXmlIndicator(QDomElement node_element, NotLeafIndicator* parent_indicator);
	bool ParseLeafIndicator(QDomElement node_element, LeafIndicator* template_indicator);
	bool ParseTemplateFormat(QDomElement node_element, TemplateFormatCollection& tformat_col);
	bool ParseInstallSources(QDomElement node_element, QVector<LeafIndicator::InstallSource>& install_sources);
	bool ParseInstallSource(QDomElement node_element, LeafIndicator::InstallSource& install_source);


private:
	bool is_match;
	bool is_install;
	WordTemplateMode cur_mode;
	QString template_path;
	DocIndicator* doc_indicator;
};


class WordTemplateMgr
{
public:
	static WordTemplateMgr& Instance();
	~WordTemplateMgr();
	void SetTemplateDir(const QString& dir);
	void RegisterTemplate(const QString& path);
	QString GetDocId(const QString& file_name);
	WordTemplate* GetWordTemplate(const QString& doc_id);
private:
	WordTemplateMgr();

	QMap<QString, WordTemplate*> word_template_map;
	QMap<QString, QString> filename2doc_id;
};
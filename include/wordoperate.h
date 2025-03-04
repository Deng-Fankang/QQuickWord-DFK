#pragma once
#include "qstring.h"
#include "qaxobject.h"
#include "qvector.h"
#include "worddefine.h"


class WordReadOperate
{
public:
	WordReadOperate(const QString& file_name);
    ~WordReadOperate();
    TitleAreaContent* CreateWordNode();

private:
    

private:
    QAxObject* word;
    QAxObject* doc;
};



class WordWriteOperate
{
public:
    WordWriteOperate(const QString& file_name);
    ~WordWriteOperate();

    //void SetTitleStyle();
    void WriteToWord(TitleAreaContent* root);

private:
    void FormatList(QAxObject* selection, WdListGalleryType gallery_type, int tid, int level, const ListTemplateFormat& list_template_format_data);
    void DoWriteToWord(TitleAreaContent* root, QAxObject* selection);

private:
    QString save_name;
    QAxObject* word;
    QAxObject* doc;
    bool is_set;
};



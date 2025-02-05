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
    TitleContentNode* CreateWordNode();

private:
    

private:
    QAxObject* word;
    QAxObject* doc;
    QVector<QString> heading_style_name;
};



class WordWriteOperate
{
public:
    WordWriteOperate(const QString& file_name);
    ~WordWriteOperate();

    void WriteToWord(TitleContentNode* root);

private:
    void DoWriteToWord(TitleContentNode* root, QAxObject* selection);

private:
    QString save_name;
    QAxObject* word;
    QAxObject* doc;
};



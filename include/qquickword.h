#pragma once

#include <QtWidgets/QMainWindow>
#include "qlayout.h"
#include "ui_qquickword.h"

class QSplitter;
class BaseFrame;

enum WordWidgetIndex
{
    WORD_IMPORT_WIDGET,
    WORD_EDIT_WIDGET,
    WORD_EXPORT_WIDGET,
};

class QQuickWord : public QMainWindow
{
    Q_OBJECT

public:
    QQuickWord(QWidget *parent = nullptr);
    ~QQuickWord();

    BaseFrame* AddSubWidget(WordWidgetIndex idx, BaseFrame* widget);
    void SetVisible(WordWidgetIndex idx, bool visible, bool retain_size=false);

    void ImportFiles();
    void ExportFiles();
    void ClearFiles();
    void MatchFiles();

public:
    Ui::QQuickWordClass ui;

private:
    QHBoxLayout* root_layout;
    QSplitter* splitter;
    QMap<WordWidgetIndex, BaseFrame*> children_widget;
};

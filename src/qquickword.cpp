#include "qquickword.h"
#include "qsplitter.h"
#include "module_manager.h"
#include "sharemgr.h"
#include "qdebug.h"
#include "wordoperate.h"
#include <iostream>


QQuickWord::QQuickWord(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    root_layout = new QHBoxLayout(ui.centralWidget);
    splitter = new QSplitter(this);
    root_layout->addWidget(splitter);
    AddSubWidget(WORD_IMPORT_WIDGET, WordImport::Module::InstancePtr()->ShowWidget(ui.centralWidget));
    AddSubWidget(WORD_EDIT_WIDGET, WordEdit::Module::InstancePtr()->ShowWidget(ui.centralWidget));
    AddSubWidget(WORD_EXPORT_WIDGET, WordExport::Module::InstancePtr()->ShowWidget(ui.centralWidget));

    connect(ui.actionimport, &QAction::triggered, this, &QQuickWord::ImportFiles);
    connect(ui.actionexport, &QAction::triggered, this, &QQuickWord::ExportFiles);
    connect(ui.actionclear, &QAction::triggered, this, &QQuickWord::ClearFiles);
}

QQuickWord::~QQuickWord()
{
    qDebug() << "QQuickWord Destroy" << endl;
}

BaseFrame* QQuickWord::AddSubWidget(WordWidgetIndex idx, BaseFrame* widget)
{

    splitter->addWidget(widget);
    children_widget[idx] = widget;
    return widget;
}

void QQuickWord::SetVisible(WordWidgetIndex idx, bool visible, bool retain_size)
{
    //QGraphicsOpacityEffect* op = new QGraphicsOpacityEffect(children_widget[idx]);
    //if (visible) {
    //    children_widget[idx]->SetOpacity(1, true);
    //}
    //else
    //{
    //    children_widget[idx]->SetOpacity(0.5, false);
    //}
    //children_widget[idx]->setGraphicsEffect(op);
}



void QQuickWord::ImportFiles()
{
    /*SetVisible(WORD_IMPORT_WIDGET, true);
    SetVisible(WORD_EDIT_WIDGET, true);*/
    QVector<QString> files = { 
        "D:\\CodeExp\\C++\\QQuickWord\\QQuickWord\\test.docx",
        //"D:\\CodeExp\\C++\\QQuickWord\\QQuickWord\\test1.docx",
    };
    ImportFilesMgr::Instance().ImportFiles(files);
}


void Test(TitleAreaContent* root) {
    if (root) {
        for (AreaContent* area : root->GetContentList()) {
            area->PrintContent();
            if (area->GetAreaType() == TITLE_AREA) {
                Test(dynamic_cast<TitleAreaContent*>(area));
            }
        }
    }
}

void QQuickWord::ExportFiles()
{
    WordWriteOperate wwrite("D:\\CodeExp\\C++\\QQuickWord\\QQuickWord\\test_write.docx");
    const vector<ImportFileData>& files_data = ImportFilesMgr::Instance().GetFilesData();
    wwrite.WriteToWord(files_data[0].root);
}

void QQuickWord::ClearFiles()
{
}

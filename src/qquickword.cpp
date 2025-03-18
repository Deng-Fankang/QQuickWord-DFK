#include "qquickword.h"
#include "qsplitter.h"
#include "qfiledialog.h"
#include "module_manager.h"
#include "sharemgr.h"
#include "qdebug.h"
#include "wordoperate.h"
#include "wordtemplate.h"
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
    //connect(ui.actionclear, &QAction::triggered, this, &QQuickWord::ClearFiles);

    QMenu* install_menu = ui.menutmplate->addMenu(QString::fromLocal8Bit("&安装"));
    QDir tmp_dir(WORD_TMP_PATH);
    for (QString file : tmp_dir.entryList(QDir::Files))
    {
        QString doc_id = WordTemplateMgr::Instance().GetDocId(file);
        QAction* action = new QAction(file.section('.', 0, 0), install_menu);
        connect(action, &QAction::triggered, this, [this, doc_id](bool checked) {
            this->InstallFile(doc_id);
        });
        install_menu->addAction(action);
    }
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
    QStringList select_paths = QFileDialog::getOpenFileNames(
        this, QString::fromLocal8Bit("选择导入文档"), "data",
        QString::fromLocal8Bit("Word文档 (*.docx *.doc)"));
    QVector<QString> files;
    for (QString select_path : select_paths)
    {
        files.push_back(select_path);
    }
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

void QQuickWord::InstallFile(QString doc_id)
{
    WordTemplate* word_template = WordTemplateMgr::Instance().GetWordTemplate(doc_id);
    if (word_template)
    {
        word_template->TryInstallContent();
        TitleAreaContent* title_area = word_template->ContructTitleAreaContent(WordTemplateMode::Install_Mode);
        TitleAreaContent::RecurNumberTitleArea(title_area);
        TitleAreaContent::RecurSetConfigFormat(title_area);
        WordExport::Module::InstancePtr()->SetTitleAreaContent(title_area);
    }
}

void QQuickWord::ClearFiles()
{
}


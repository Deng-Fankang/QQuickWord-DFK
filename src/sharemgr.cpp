#include "sharemgr.h"
#include "qfileinfo.h"
#include "qdebug.h"
#include "wordoperate.h"

QQuickWord* main_window;


void ShowMainWindow() {
    if (!main_window) {
        main_window = new QQuickWord();
        main_window->setAttribute(Qt::WA_DeleteOnClose);
    }
    main_window->show();
}

ImportFileData CreateImportFile(QString file_path) {


    WordReadOperate wread(file_path);
    TitleContentNode* root = wread.CreateWordNode();
    QFileInfo file_info(file_path);
    return ImportFileData(file_info.fileName(), root);
}


ImportFilesMgr::ImportFilesMgr() {
    a_ = 1;
}

ImportFilesMgr& ImportFilesMgr::Instance()
{
    static ImportFilesMgr mgr;
    return mgr;
}

const vector<ImportFileData>& ImportFilesMgr::GetFilesData() const
{
    return m_files_data;
}

void ImportFilesMgr::ImportFiles(QVector<QString> files)
{
    m_files_data.clear();
    for (const QString& file : files) {
        m_files_data.push_back(CreateImportFile(file));
    }
    emit importFilesChange();
}


const int& ImportFilesMgr::GetA() const
{
    return a_;
}



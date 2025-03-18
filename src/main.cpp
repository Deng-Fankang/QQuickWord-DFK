#include "sharemgr.h"
#include "module_manager.h"
#include "wordconfig.h"
#include <QtWidgets/QApplication>
#include <memory>
#include "wordtemplate.h"

using namespace std;

int main(int argc, char *argv[])
{
    
    if (WordConfig::InitWordConfig())
    {
        WordTemplateMgr::Instance().SetTemplateDir(WORD_TMP_PATH);
        QApplication a(argc, argv);
        ShowMainWindow();
        ModuleManager::Instance().Start();
        int ret = a.exec();
        ModuleManager::Instance().Stop();
        return ret;
    }
    return -1;
}

#include "sharemgr.h"
#include "module_manager.h"
#include <QtWidgets/QApplication>
#include <memory>
#include "wordtemplate.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ShowMainWindow();
    WordTemplateMgr::Instance().RegisterTemplate("import.xml");

    ModuleManager::Instance().Start();
    int ret = a.exec();
    ModuleManager::Instance().Stop();
    return ret;
}

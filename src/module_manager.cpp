#include <vector>
#include "basemodule.h"
#include "module_manager.h"

WordImport::Module ModuleManager::g_word_import;
WordExport::Module ModuleManager::g_word_export;
WordEdit::Module ModuleManager::g_word_edit;

const std::vector<BaseModule*> MODULE_PTR_LIST = {
	&ModuleManager::g_word_import,
	&ModuleManager::g_word_edit,
	&ModuleManager::g_word_export,
};


ModuleManager::ModuleManager()
{

}

ModuleManager::~ModuleManager() {

}

ModuleManager& ModuleManager::Instance()
{
	static ModuleManager module_mgr;
	return module_mgr;
}

void ModuleManager::Start()
{
	for (auto it = MODULE_PTR_LIST.begin(); it != MODULE_PTR_LIST.end(); it++) {
		(*it)->Start();
	}
}


void ModuleManager::Stop()
{
	for (auto it = MODULE_PTR_LIST.rbegin(); it != MODULE_PTR_LIST.rend(); it++) {
		(*it)->Stop();
	}
}



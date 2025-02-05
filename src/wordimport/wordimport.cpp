#include "wordimport.h"
#include "wordimportwidget.h"
#include "sharefunc.h"
#include <iostream>
using namespace std;

SINGLETON_INIT(WordImport::Module);

namespace WordImport {

	Module::Module()
	{
	}

	Module::~Module()
	{
	}
	BaseFrame* Module::ShowWidget(QWidget* parent) {
		if (w == nullptr) {
			w = new WordImportWidget(parent);
		}
		w->show();
		return w;
	}


	void Module::Start()
	{

		cout << "wordimport start" << endl;
	}
	void Module::Stop()
	{
		cout << "wordimport stop" << endl;
	}


	TreeItem* Module::GetCurFileTitleTree()
	{
		return title_tree_buffer[m_ui_data.current_file_idx];
	}


	void Module::ClearBuffer()
	{
		file_list_buffer.clear();
		for (TreeItem* root : title_tree_buffer) {
			DeleteTitleTree(root);
		}
		title_tree_buffer.clear();
	}

	ImportTreeModel* Module::GetImportTreeModel()
	{
		if (w)
		{
			return dynamic_cast<WordImportWidget*>(w)->GetImportTreeModel();
		}
		return nullptr;
	}
}
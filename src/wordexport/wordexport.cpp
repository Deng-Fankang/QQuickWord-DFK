#include "wordexport.h"
#include "wordexportwidget.h"
#include<iostream>
using namespace std;

SINGLETON_INIT(WordExport::Module);

namespace WordExport {

	Module::Module()
	{
	}

	Module::~Module()
	{
	}

	BaseFrame* Module::ShowWidget(QWidget* parent) {
		if (w == nullptr) {
			w = new WordExportWidget(parent);
		}
		w->show();
		return w;
	}

	void Module::Start()
	{
		cout << "wordexport start" << endl;
	}
	void Module::Stop()
	{
		cout << "wordexport stop" << endl;
	}
	void Module::SetTitleAreaContent(TitleAreaContent* title_area)
	{
		if (title_area == nullptr)
		{
			return;
		}
		dynamic_cast<WordExportWidget*>(w)->SetTitleAreaContent(title_area);
	}
}
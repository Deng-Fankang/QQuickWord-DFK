#include "wordedit.h"
#include "wordeditwidget.h"
#include "qwidget.h"
#include<iostream>
using namespace std;

SINGLETON_INIT(WordEdit::Module);

namespace WordEdit {
	Module::Module()
	{
	}

	Module::~Module()
	{
	}

	BaseFrame* Module::ShowWidget(QWidget* parent) {
		if (w == nullptr) {
			w = new WordEditWidget(parent);
		}
		w->show();
		return w;
	}

	void Module::Start()
	{
		cout << "wordedit start" << endl;
	}
	void Module::Stop()
	{
		cout << "wordedit stop" << endl;
	}

	void Module::SetImportEditContent(const AreaContent* area_content)
	{
		if (area_content == nullptr)
		{
			return;
		}
		((WordEditWidget*)w)->SetImportEditContent(area_content);
	}

	void Module::SetExportEditContent(const AreaContent* area_content)
	{
		if (area_content == nullptr)
		{
			return;
		}
		((WordEditWidget*)w)->SetExportEditContent(area_content);
	}
}

#pragma once
#include "basemodule.h"
#include "singleton.h"

class TitleContentNode;

namespace WordEdit {
	class Module: public BaseModule, public Singleton<Module>
	{
	public:
		Module();
		~Module();
		BaseFrame* ShowWidget(QWidget* parent = nullptr);
		void Start();
		void Stop();

		void SetImportEditContent(const TitleContentNode* node);
		void SetExportEditContent(const TitleContentNode* node);

	public:

	private:

	};
}


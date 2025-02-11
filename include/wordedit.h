#pragma once
#include "basemodule.h"
#include "singleton.h"

class TitleAreaContent;
class AreaContent;

namespace WordEdit {
	class Module: public BaseModule, public Singleton<Module>
	{
	public:
		Module();
		~Module();
		BaseFrame* ShowWidget(QWidget* parent = nullptr);
		void Start();
		void Stop();

		void SetImportEditContent(const AreaContent* area_content);
		void SetExportEditContent(const AreaContent* area_content);

	public:

	private:

	};
}


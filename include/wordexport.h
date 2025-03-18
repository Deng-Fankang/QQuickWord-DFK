#pragma once
#include "basemodule.h"
#include "singleton.h"

class TitleAreaContent;
namespace WordExport {

	class Module: public BaseModule, public Singleton<Module>
	{
	public:
		Module();
		~Module();
		BaseFrame* ShowWidget(QWidget* parent = nullptr);
		void Start();
		void Stop();

		void SetTitleAreaContent(TitleAreaContent* title_area);
		//TreeItem* GetExport

	public:

	private:
		static int a;
	};
}
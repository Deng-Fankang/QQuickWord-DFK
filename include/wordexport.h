#pragma once
#include "basemodule.h"
#include "singleton.h"

namespace WordExport {

	class Module: public BaseModule, public Singleton<Module>
	{
	public:
		Module();
		~Module();
		BaseFrame* ShowWidget(QWidget* parent = nullptr);
		void Start();
		void Stop();

		//TreeItem* GetExport

	public:

	private:
		static int a;
	};
}
#pragma once
#include "basemodule.h"
#include "singleton.h"
#include "qvariant.h"
#include "qmap.h"

class TreeItem;
class ImportTreeModel;
namespace WordImport {

	class WordImportWidget;
	class Module: public BaseModule, public Singleton<Module>
	{
		friend class WordImportWidget;
		class UIData 
		{
			friend class WordImportWidget;
			friend class Module;
			int current_file_idx;
		};
	public:
		Module();
		~Module();
		BaseFrame* ShowWidget(QWidget* parent = nullptr);
		void Start();
		void Stop();

		TreeItem* GetCurFileTitleTree();
		void Module::ClearBuffer();

		ImportTreeModel* GetImportTreeModel();

	private:
		UIData m_ui_data;
		QVector<QMap<int, QVariant>> file_list_buffer;
		QVector<TreeItem*> title_tree_buffer;
	};
}
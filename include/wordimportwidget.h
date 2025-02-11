#pragma once

#include <QWidget>
#include "qboxlayout.h"
#include "baseframe.h"
#include "ui_wordimportwidget.h"

class QUListModel;
class ImportTreeModel;
class QListView;
class QTreeView;
class TreeItem;
class TimerTest;

namespace WordImport {

	class WordImportWidget : public BaseFrame
	{
		Q_OBJECT

	public:
		WordImportWidget(QWidget* parent = nullptr);
		~WordImportWidget();

		inline ImportTreeModel* GetImportTreeModel() {
			return tree_model;
		}

	private:
		void SetWidget();

		void OnSelectFile(const QModelIndex& current, const QModelIndex& previous);
		void OnSelectItem(const QModelIndex& current, const QModelIndex& previous);
		void ImportFilesChange();

	private:
		Ui::WordImportWidgetClass ui;
		QUListModel* list_model;
		QListView* file_list_view;
		ImportTreeModel* tree_model;
		QTreeView* title_tree_view;
		QHBoxLayout* hbox_layout;
	};
};

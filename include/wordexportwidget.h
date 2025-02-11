#pragma once

#include <QWidget>
#include "baseframe.h"
#include "wordexportmodel.h"
#include "exportview.h"
#include "ui_wordexportwidget.h"

namespace WordExport {
	class WordExportWidget : public BaseFrame
	{
		Q_OBJECT

	public:
		WordExportWidget(QWidget* parent = nullptr);
		~WordExportWidget();

	private:
		void SetWidget();
		void PrintTree(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles);
		void OnSelectTest(const QModelIndex& current, const QModelIndex& previous);
		void OnSelectItem(const QModelIndex& current, const QModelIndex& previous);

	private:
		TreeItem* root;
		TreeItem* root_edit_able;
		ExportTitleView* title_tree_view;
		ExportTreeModel* title_tree_model;
		Ui::WordExportWidgetClass ui;
	};
}
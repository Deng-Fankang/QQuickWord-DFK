#include "exportview.h"
#include "qevent.h"
#include "qfiledialog.h"
#include "wordexportmodel.h"
#include "worddefine.h"
#include "wordoperate.h"
#include "qmenu.h"
#include "qdebug.h"

#define EXPORT_MODEL_PTR dynamic_cast<ExportTreeModel*>(model())


ExportTitleView::ExportTitleView(QWidget* parent)
	:QTreeView(parent)
{
	setDragEnabled(true);
	setDragDropMode(QAbstractItemView::DragDrop);
	setDropIndicatorShown(true);
	//connect(model(), &ExportTreeModel::rowsMoved, this, &ExportTitleView::SlotRowsMoved);
}

void ExportTitleView::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) {
		QTreeView::mousePressEvent(event);
	}
	else if (event->button() == Qt::RightButton) {
		operate_index = indexAt(event->pos());
		if (operate_index.isValid()) {
			TreeItem* item = static_cast<ExportTreeModel*>(model())->TreeItemPtr(operate_index);
			qDebug() << item->user_data[Qt::DisplayRole].toString() << endl;
			ShowMenuPop(event->pos());
		}
	}
}

void ExportTitleView::dropEvent(QDropEvent* event)
{
	selectionModel()->clearSelection();
	QTreeView::dropEvent(event);
}

void ExportTitleView::dragMoveEvent(QDragMoveEvent* event)
{
	qDebug() << event->pos() << endl;
	QAbstractItemView::dragMoveEvent(event);
}

void ExportTitleView::ShowMenuPop(const QPoint& pos)
{
	if (!operate_index.isValid()) return;

	QMenu* menu = new QMenu(this);
	if (operate_index.parent().parent() == QModelIndex()) {
		QAction* append_child_node = new QAction(QString::fromLocal8Bit("&添加子节点"), menu);
		QAction* export_node = new QAction(QString::fromLocal8Bit("&导出文档"), menu);
		connect(append_child_node, &QAction::triggered, this, &ExportTitleView::SlotAppendChildItem);
		connect(export_node, &QAction::triggered, this, &ExportTitleView::ExportTitleArea);
		menu->addAction(append_child_node);
		menu->addAction(export_node);
	}
	else
	{
		QAction* append_node = new QAction(QString::fromLocal8Bit("&添加同级标题"), menu);
		QAction* append_child_node = new QAction(QString::fromLocal8Bit("&添加子标题"), menu);
		QAction* delete_node = new QAction(QString::fromLocal8Bit("&删除标题"), menu);
		connect(append_node, &QAction::triggered, this, &ExportTitleView::SlotAppendItem);
		connect(append_child_node, &QAction::triggered, this, &ExportTitleView::SlotAppendChildItem);
		connect(delete_node, &QAction::triggered, this, &ExportTitleView::SlotDeleteItem);
		menu->addAction(append_node);
		menu->addAction(append_child_node);
		menu->addAction(delete_node);
	}
	menu->exec(mapToGlobal(pos));
}

void ExportTitleView::SlotDeleteItem(bool checked)
{
	if (operate_index.isValid()) 
	{
		model()->removeRow(operate_index.row(), operate_index.parent());
	}
}

void ExportTitleView::SlotAppendItem(bool checked)
{
	if (operate_index.isValid())
	{
		int row = operate_index.row();
		model()->insertRow(row, operate_index.parent());

		TitleAreaContent* child_title = new TitleAreaContent(QString::fromLocal8Bit("新标题"));
		model()->setData(operate_index.parent().child(row, 0), QVariant::fromValue<void*>(child_title), Qt::TitleContentRole);

	}
}

void ExportTitleView::SlotAppendChildItem(bool checked)
{
	if (operate_index.isValid()) {
		int count = model()->rowCount(operate_index);
		model()->insertRow(count, operate_index);

		TitleAreaContent* child_title = new TitleAreaContent(QString::fromLocal8Bit("新标题"));
		model()->setData(operate_index.child(count, 0), QVariant::fromValue<void*>(child_title), Qt::TitleContentRole);

	}
}

void ExportTitleView::ExportTitleArea(bool checked)
{
	if (operate_index.isValid() && operate_index.parent().parent() == QModelIndex())
	{
		TreeItem* tree_item = EXPORT_MODEL_PTR->TreeItemPtr(operate_index);
		AreaContent* title_area = tree_item->item_data->GetAreaContent();
		if (title_area->GetAreaType() == TITLE_AREA)
		{
			QString fileName = QFileDialog::getSaveFileName(this,
				QString::fromLocal8Bit("导出文档"), "data",
				QString::fromLocal8Bit("Word文档 (*.docx *.doc)"));
			WordWriteOperate wwrite(fileName);
			wwrite.WriteToWord(dynamic_cast<TitleAreaContent*>(title_area));
		}
	}
}



#include "exportview.h"
#include "qevent.h"
#include "wordexportmodel.h"
#include "worddefine.h"
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
	if (operate_index.parent() == QModelIndex()) {
		QAction* append_child_node = new QAction(QString::fromLocal8Bit("&添加子节点"), this);
		connect(append_child_node, &QAction::triggered, this, &ExportTitleView::SlotAppendChildItem);
		menu->addAction(append_child_node);
	}
	else
	{
		QAction* append_node = new QAction(QString::fromLocal8Bit("&添加同级标题"), this);
		QAction* append_child_node = new QAction(QString::fromLocal8Bit("&添加子标题"), this);
		QAction* delete_node = new QAction(QString::fromLocal8Bit("&删除标题"), this);
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

		TitleContentNode* child_item_node = new TitleContentNode(QString::fromLocal8Bit("新标题"));
		model()->setData(operate_index.sibling(row, 0), QVariant::fromValue<void*>(child_item_node), Qt::TitleContentRole);

		TreeItem* parent_item = EXPORT_MODEL_PTR->TreeItemPtr(operate_index.parent());

		TitleContentNode* parent_item_node = dynamic_cast<TitleItemData*>(parent_item->item_data)->GetTitleContentNode();
		TitleAreaContent* content = new TitleAreaContent(child_item_node);
		parent_item_node->InsertAreaContent(content, row);
	}
}

void ExportTitleView::SlotAppendChildItem(bool checked)
{
	if (operate_index.isValid()) {
		int count = model()->rowCount(operate_index);
		model()->insertRow(count, operate_index);

		TitleContentNode* child_item_node = new TitleContentNode(QString::fromLocal8Bit("新标题"));
		model()->setData(operate_index.child(count, 0), QVariant::fromValue<void*>(child_item_node), Qt::TitleContentRole);

		TreeItem* parent_item = EXPORT_MODEL_PTR->TreeItemPtr(operate_index);

		TitleContentNode* parent_item_node = dynamic_cast<TitleItemData*>(parent_item->item_data)->GetTitleContentNode();
		TitleAreaContent* content = new TitleAreaContent(child_item_node);
		parent_item_node->InsertAreaContent(content);
	}
}



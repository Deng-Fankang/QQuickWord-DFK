#pragma once
#include "qtreeview.h"


class ExportTitleView: public QTreeView
{
	friend class QTreeViewPrivate;
public:
	ExportTitleView(QWidget* parent = nullptr);

protected:
	void mousePressEvent(QMouseEvent* event);
#if QT_CONFIG(draganddrop)
	void dropEvent(QDropEvent* event);
	void dragMoveEvent(QDragMoveEvent* event);
#endif

private:
	void ShowMenuPop(const QPoint& pos);
	void SlotDeleteItem(bool checked);
	void SlotAppendItem(bool checked);
	void SlotAppendChildItem(bool checked);

private:
	QModelIndex operate_index;
};

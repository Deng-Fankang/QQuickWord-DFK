#include "exportdelegate.h"

void ExportTitleDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	TreeTitleItemDelegate::paint(painter, option, index);
}

QSize ExportTitleDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	return TreeTitleItemDelegate::sizeHint(option, index);
}

QWidget* ExportTitleDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	return TreeTitleItemDelegate::createEditor(parent, option, index);
}

void ExportTitleDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
	TreeTitleItemDelegate::setEditorData(editor, index);
}

void ExportTitleDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	TreeTitleItemDelegate::setModelData(editor, model, index);
}

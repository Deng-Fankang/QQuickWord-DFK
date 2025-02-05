#include "exportdelegate.h"
#include "worddefine.h"
#include "qpainter.h"
#include "qdebug.h"

void ExportTitleDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QStyleOptionViewItem opt = option;
	opt.features |= QStyleOptionViewItem::HasDecoration;
	int size = qMin(opt.rect.height(), opt.rect.width());
	opt.decorationSize = QSize(size, size);
	opt.icon = QIcon("title_content.png");
	QStyledItemDelegate::paint(painter, opt, index);
	/*if (index.data(Qt::UserRole) == TITLE_CONTENT) {
		opt.icon = QIcon("title_content.png");
		QStyledItemDelegate::paint(painter, opt, index);
	}
	else
	{
		opt.icon = QIcon("content.png");
		if (opt.state & QStyle::State_MouseOver) {
			painter->fillRect(opt.rect, Qt::yellow);
		}
		else
		{
			painter->fillRect(opt.rect, Qt::white);
		}
		QStyledItemDelegate::paint(painter, opt, index);
	}*/
}

QSize ExportTitleDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	return QStyledItemDelegate::sizeHint(option, index);
}

QWidget* ExportTitleDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	return QStyledItemDelegate::createEditor(parent, option, index);
}

void ExportTitleDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
	QStyledItemDelegate::setEditorData(editor, index);
}

void ExportTitleDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	QStyledItemDelegate::setModelData(editor, model, index);
}

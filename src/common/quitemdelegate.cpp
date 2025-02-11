#include "quitemdelegate.h"
#include "qutreemodel.h"
#include <iostream>

void TreeTitleItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QStyleOptionViewItem opt = option;
	opt.features |= QStyleOptionViewItem::HasDecoration;
	int size = qMin(opt.rect.height(), opt.rect.width());
	opt.decorationSize = QSize(size, size);

	TreeItem* item = dynamic_cast<const QUTreeModel*>(index.model())->TreeItemPtr(index);

	AreaType area_type = item->item_data->GetAreaContent()->GetAreaType();
	if (area_type == TEXT) {
		opt.icon = QIcon("config/image/text.png");
	}
	else if (area_type == LIST)
	{
		opt.icon = QIcon("config/image/list.png");
	}
	else if (area_type == IMAGE)
	{
		opt.icon = QIcon("config/image/image.png");
	}
	else if (area_type == TABLE)
	{
		opt.icon = QIcon("config/image/table.png");
	}
	else
	{
		opt.icon = QIcon("config/image/title_content.png");
		if (opt.state & QStyle::State_MouseOver) {
			painter->fillRect(opt.rect, Qt::yellow);
		}
		else
		{
			painter->fillRect(opt.rect, Qt::white);
		}
	}
	QStyledItemDelegate::paint(painter, opt, index);
}

QSize TreeTitleItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	return QStyledItemDelegate::sizeHint(option, index);
}

QWidget* TreeTitleItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	return QStyledItemDelegate::createEditor(parent, option, index);
}

void TreeTitleItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
	QStyledItemDelegate::setEditorData(editor, index);
}

void TreeTitleItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	QStyledItemDelegate::setModelData(editor, model, index);
}


QUWidgetDelegate::QUWidgetDelegate(WidgetHelper* helper_, QObject* parent) :
	QStyledItemDelegate(parent)
{
	m_helper = helper_;
	m_helper->createWidget();
}

void QUWidgetDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	//if (option.state & QStyle::State_MouseOver) {
	//	std::cout << "over" << index.row() << std::endl;
	//}
	std::cout << "over" << index.row() << option.state << std::endl;
	m_helper->w->resize(option.rect.size());
	m_helper->setWidgetData(index);

	painter->save();
	painter->translate(option.rect.topLeft());
	m_helper->w->render(painter);
	painter->restore();
}

QSize QUWidgetDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	m_helper->setWidgetData(index);
	return m_helper->w->sizeHint();
}
#include "quitemdelegate.h"
#include "qpainter.h"
#include <iostream>

QUWidgetDelegate::QUWidgetDelegate(WidgetHelper* helper_, QObject* parent):
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



#pragma 
#include "qstyleditemdelegate.h"


class WidgetHelper {
	friend class QUWidgetDelegate;
public:
	WidgetHelper() { w = nullptr; };
	virtual void createWidget() = 0;
	virtual void setWidgetData(const QModelIndex& index) const = 0;

protected:
	QWidget* w;
};


class QUWidgetDelegate: public QStyledItemDelegate
{
public:
	explicit QUWidgetDelegate(WidgetHelper* helper_, QObject* parent = nullptr);
	void paint(QPainter* painter,
		const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	QSize sizeHint(const QStyleOptionViewItem& option,
		const QModelIndex& index) const override;
public:
	WidgetHelper* m_helper;
};


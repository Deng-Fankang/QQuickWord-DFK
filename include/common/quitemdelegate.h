#pragma 
#include "qstyleditemdelegate.h"
#include "qpainter.h"


class TreeTitleItemDelegate : public QStyledItemDelegate
{
public:
	void paint(QPainter* painter,
		const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	QSize sizeHint(const QStyleOptionViewItem& option,
		const QModelIndex& index) const override;

	QWidget* createEditor(QWidget* parent,
		const QStyleOptionViewItem& option,
		const QModelIndex& index) const override;

	void setEditorData(QWidget* editor, const QModelIndex& index) const override;
	void setModelData(QWidget* editor,
		QAbstractItemModel* model,
		const QModelIndex& index) const override;
};


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


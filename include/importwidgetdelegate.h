#pragma once
#include "quitemdelegate.h"

class FileWidgetHelper : public WidgetHelper
{
public:
	void createWidget() override;
	void setWidgetData(const QModelIndex& index) const override;
};

class FileItemDelegate: public QStyledItemDelegate
{
public:
	void paint(QPainter* painter,
		const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	QSize sizeHint(const QStyleOptionViewItem& option,
		const QModelIndex& index) const override;

};


class ImportTitleDelegate : public TreeTitleItemDelegate
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




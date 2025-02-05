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


class TitleItemDelegate : public QStyledItemDelegate
{
public:
	void paint(QPainter* painter,
		const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	QSize sizeHint(const QStyleOptionViewItem& option,
		const QModelIndex& index) const override;
};




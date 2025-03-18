#pragma once
#include "qlistview.h"

class FileListView : public QListView
{
	Q_OBJECT
public:
	explicit FileListView(QWidget* parent = nullptr);

	void mousePressEvent(QMouseEvent* event) override;

	void ShowMenuPop(QPoint pos);

private slots:
	void TryMatchTemplate(QString doc_id);
private:
	QModelIndex operate_index;
};
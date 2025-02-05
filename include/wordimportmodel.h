#pragma once
#include "qutreemodel.h"

class ImportTreeModel: public QUTreeModel
{
public:
	ImportTreeModel(TreeItem* root = nullptr, QObject* parent = nullptr);

	Qt::ItemFlags flags(const QModelIndex& index) const;
	virtual QMimeData* mimeData(const QModelIndexList& indexes) const;

	~ImportTreeModel();


private:

};
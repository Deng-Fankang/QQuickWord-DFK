#pragma once
#include "qabstractitemmodel.h"
#include "qvector.h"
#include "treeitem.h"


class QUTreeModel: public QAbstractItemModel
{
public:
	explicit QUTreeModel(TreeItem* root = nullptr, QObject* parent = nullptr);

	QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;

	QModelIndex parent(const QModelIndex& child) const override;
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

	TreeItem* TreeItemPtr(const QModelIndex& index) const;
	void updateData(TreeItem* root);

public:
	TreeItem* m_root;
};

extern ITreeItemData* GetTreeItemDataPtr(QModelIndex index);


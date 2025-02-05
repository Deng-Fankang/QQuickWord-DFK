#include "qutreemodel.h"
#include "worddefine.h"

QUTreeModel::QUTreeModel(TreeItem* root, QObject* parent)
	:QAbstractItemModel(parent)
{
	m_root = root;
}

QModelIndex QUTreeModel::index(int row, int column, const QModelIndex& parent) const
{
	if (!hasIndex(row, column, parent)) {
		return QModelIndex();
	}
	TreeItem* parent_item = parent.isValid() ? static_cast<TreeItem*>(parent.internalPointer()) : m_root;
	if (parent_item && parent_item->children[row]) {
		return createIndex(row, column, parent_item->children[row]);
	}
	return QModelIndex();
}

QModelIndex QUTreeModel::parent(const QModelIndex& child) const
{
	if (child.isValid()) {
		TreeItem* child_item = static_cast<TreeItem*>(child.internalPointer());
		TreeItem* parent_item = child_item->parent;
		if (parent_item) {
			return createIndex(parent_item->row(), 0, parent_item);
		}
	}
	return QModelIndex();
}

int QUTreeModel::rowCount(const QModelIndex& parent) const
{
	if (!m_root) {
		return 0;
	}
	if (parent.isValid()) {
		TreeItem* parent_item = static_cast<TreeItem*>(parent.internalPointer());
		return parent_item->children.size();
	}
	else
	{
		return m_root->children.size();
	}
}

int QUTreeModel::columnCount(const QModelIndex& parent) const
{
	return 1;
}

QVariant QUTreeModel::data(const QModelIndex& index, int role) const
{
	if (index.isValid()) {
		TreeItem* item = static_cast<TreeItem*>(index.internalPointer());
		return item->user_data[role];
	}
	return QVariant();
}

TreeItem* QUTreeModel::TreeItemPtr(const QModelIndex& index) const
{
	if(!m_root) return nullptr;
	if (index.isValid()) {
		TreeItem* item = static_cast<TreeItem*>(index.internalPointer());
		return item;
	}
	else
	{
		return m_root;
	}
}

void QUTreeModel::updateData(TreeItem* root)
{
	beginResetModel();
	m_root = root;
	endResetModel();
}

ITreeItemData* GetTreeItemDataPtr(QModelIndex index)
{
	ITreeItemData* item_data = dynamic_cast<const QUTreeModel*>(index.model())->TreeItemPtr(index)->item_data;
	return item_data;
}

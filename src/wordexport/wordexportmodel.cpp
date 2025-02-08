#include "wordexportmodel.h"
#include "qmimedata.h"
#include "qdatastream.h"
#include "wordimportmodel.h"
#include "worddefine.h"
#include "sharefunc.h"
#include "global.h"
#include "module_manager.h"
#include "qdebug.h"

ExportTreeModel::ExportTreeModel(TreeItem* root, QObject* parent)
	:QUTreeModel(root, parent)
{

}

bool ExportTreeModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	if (index.isValid())
	{
		TreeItem* item = TreeItemPtr(index);
		if (role == Qt::EditRole)
		{
			QString val_str = value.toString();
			item->user_data[Qt::DisplayRole] = val_str;
			item->user_data[Qt::EditRole] = val_str;
		}
		else if(role == Qt::TitleContentRole)
		{
			TitleContentNode* node = static_cast<TitleContentNode*>(value.value<void*>());
			item->item_data = new TitleItemData(item, node);
		}
		emit dataChanged(index, index);
		return true;
	}
	return false;
}

Qt::ItemFlags ExportTreeModel::flags(const QModelIndex& index) const
{
	if (index.parent() == QModelIndex())
	{
		return QAbstractItemModel::flags(index) | Qt::ItemIsDropEnabled;
	}
	return Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | QAbstractItemModel::flags(index);
}

bool ExportTreeModel::insertRows(int row, int count, const QModelIndex& parent)
{
	beginInsertRows(parent, row, row + count - 1);
	TreeItem* parent_item = dynamic_cast<TreeItem*>(TreeItemPtr(parent));
	for (int i = row; i < row + count; i++)
	{
		TreeItem* item = new TreeItem(parent_item, i);
		item->user_data[Qt::DisplayRole] = QString::fromLocal8Bit("新标题");
		item->user_data[Qt::EditRole] = QString::fromLocal8Bit("新标题");
	}
	endInsertRows();
	return true;
}

bool ExportTreeModel::removeRows(int row, int count, const QModelIndex& parent)
{
	beginRemoveRows(parent, row, row + count - 1);
	TreeItem* parent_item = TreeItemPtr(parent);
	parent_item->RemoveChildren(row, count);
	endRemoveRows();
	return true;
}

bool ExportTreeModel::moveRows(const QModelIndex& sourceParent, int sourceRow, int count, const QModelIndex& destinationParent, int destinationChild)
{
	beginMoveRows(sourceParent, sourceRow, sourceRow + count - 1, destinationParent, destinationChild);
	TreeItem* source_parent_item = TreeItemPtr(sourceParent);
	TreeItem* dst_parent_item = TreeItemPtr(destinationParent);

	for (int i = 0; i < count; i++) 
	{
		TreeItem* move_item = source_parent_item->children[sourceRow + i];
		
		if (destinationChild + i < rowCount(destinationParent)) {
			move_item->SetParent(dst_parent_item, destinationChild + i);
		}
		else
		{
			move_item->SetParent(dst_parent_item);
		}
	}
	endMoveRows();
	return true;
}

QMimeData* ExportTreeModel::mimeData(const QModelIndexList& indexes) const
{
	QMimeData* data = new QMimeData();
	QByteArray encoded;
	QDataStream stream(&encoded, QIODevice::WriteOnly);
	QModelIndex* index = new QModelIndex(indexes.at(0));
	stream << (quint64)index;
	data->setData(EXPORT_DROP_STR, encoded);
	return data;
}

bool ExportTreeModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent)
{
	TreeItem* parent_item = TreeItemPtr(parent);
	if (data->hasFormat(EXPORT_DROP_STR))
	{
		QByteArray encoded = data->data(EXPORT_DROP_STR);
		QDataStream stream(&encoded, QIODevice::ReadOnly);
		quint64 i = 0;
		stream >> i;
		QModelIndex* drag_index = reinterpret_cast<QModelIndex*>(i);
		TreeItem* drop_item = TreeItemPtr(*drag_index);
		if (row == -1 && column == -1) {
			if (drag_index->parent() != parent && (*drag_index) != parent)
			{
				moveRow(drag_index->parent(), drag_index->row(), parent, rowCount(parent));
			}
		}
		else
		{
			moveRow(drag_index->parent(), drag_index->row(), parent, row);
		}
		delete drag_index;
		return true;
	}
	else if(data->hasFormat(IMPORT_DROP_STR))
	{
		QByteArray encoded = data->data(IMPORT_DROP_STR);
		QDataStream stream(&encoded, QIODevice::ReadOnly);
		quint64 i = 0;
		stream >> i;
		QModelIndex* drag_index = reinterpret_cast<QModelIndex*>(i);
		
		ImportTreeModel* title_tree_model = WordImport::Module::InstancePtr()->GetImportTreeModel();
		TreeItem* drop_item = title_tree_model->TreeItemPtr(*drag_index);

		
		if (drop_item->item_data->data_type == ITreeItemData::ItemDataType::Title_Data)
		{
			TitleContentNode* drop_node = dynamic_cast<TitleItemData*>(drop_item->item_data)->GetTitleContentNode();
			TitleContentNode* drop_node_cp = GetTitleContentNode(drop_node);

			TitleContentNode* parent_item_node = dynamic_cast<TitleItemData*>(parent_item->item_data)->GetTitleContentNode();
			if (row == -1 && column == -1)
			{
				//TreeItem* tree_item = GetTitleTreeFromNode(drop_node_cp);
				int insert_row = rowCount(parent);
				insertTreeRows(insert_row, parent, drop_node_cp);
				// 父标题节点插入新增树内容
				TitleAreaContent* content = new TitleAreaContent(drop_node_cp);
				parent_item_node->InsertAreaContent(content);
			}
			else
			{
				insertTreeRows(row, parent, drop_node_cp);
				TitleAreaContent* content = new TitleAreaContent(drop_node_cp);
				parent_item_node->InsertAreaContent(content, row);
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}

Qt::DropActions ExportTreeModel::supportedDropActions() const
{
	return Qt::MoveAction | Qt::CopyAction;
}

QStringList ExportTreeModel::mimeTypes() const
{
	QStringList ss;
	ss << EXPORT_DROP_STR << IMPORT_DROP_STR;
	return ss;
}

bool ExportTreeModel::canDropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) const
{
	Q_UNUSED(row)
	Q_UNUSED(column)

	if (!(action & supportedDropActions()))
		return false;
	if (parent == QModelIndex()) {
		return false;
	}
	const QStringList modelTypes = mimeTypes();
	for (int i = 0; i < modelTypes.count(); ++i) {
		if (data->hasFormat(modelTypes.at(i)))
			return true;
	}
	return false;
}

void ExportTreeModel::insertTreeRows(int insert_row, const QModelIndex& parent, TitleContentNode* root_node)
{//root_node 需要有完整的父子关系
	insertRow(insert_row, parent);
	QModelIndex insert_index = index(insert_row, 0, parent);
	QMap<int, QVariant> roles = {
		{Qt::EditRole, root_node->title},
		{Qt::TitleContentRole, QVariant::fromValue((void*)root_node)},
	};
	setItemData(insert_index, roles);
	int i = 0;
	for (auto it = root_node->GetChildTitleList().cbegin(); it != root_node->GetChildTitleList().cend(); it++, i++)
	{
		assert(root_node == (*it)->GetParent());
		insertTreeRows(i, insert_index, (*it));
	}
}

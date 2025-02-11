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
		switch (role)
		{
		case Qt::DisplayRole:
		{
			QString val_str = value.toString();
			item->user_data[Qt::DisplayRole] = val_str;
			break;
		}
		case Qt::EditRole:
		{
			QString val_str = value.toString();
			item->user_data[Qt::DisplayRole] = val_str;
			item->user_data[Qt::EditRole] = val_str;
			break;
		}
		case Qt::TitleContentRole:
		case Qt::TextContentRole:
		case Qt::TableContentRole:
		case Qt::ImageContentRole:
		case Qt::ListContentRole:
		{
			TreeItem* patent_item = TreeItemPtr(index.parent());
			TitleAreaContent* parent_area = dynamic_cast<TitleAreaContent*>(patent_item->item_data->GetAreaContent());
			
			if (role == Qt::TitleContentRole)
			{
				TitleAreaContent* content = static_cast<TitleAreaContent*>(value.value<void*>());
				item->item_data = new TitleItemData(item, content);
				parent_area->InsertAreaContent(content, index.row());
			}
			else if (role == Qt::TextContentRole)
			{
				TextAreaContent* content = static_cast<TextAreaContent*>(value.value<void*>());
				item->item_data = new TextItemData(item, content);
				parent_area->InsertAreaContent(content, index.row());
			}
			else if (role == Qt::TableContentRole)
			{
				TableAreaContent* content = static_cast<TableAreaContent*>(value.value<void*>());
				item->item_data = new TableItemData(item, content);
				parent_area->InsertAreaContent(content, index.row());
			}
			else if (role == Qt::ImageContentRole)
			{
				ImageAreaContent* content = static_cast<ImageAreaContent*>(value.value<void*>());
				item->item_data = new ImageItemData(item, content);
				parent_area->InsertAreaContent(content, index.row());
			}
			else
			{//List
				ListAreaContent* content = static_cast<ListAreaContent*>(value.value<void*>());
				item->item_data = new ListItemData(item, content);
				parent_area->InsertAreaContent(content, index.row());
			}
			break;
		}
		
		default:
			break;
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
		return QAbstractItemModel::flags(index);
	}
	if (index.parent().parent() == QModelIndex())
	{
		return QAbstractItemModel::flags(index) | Qt::ItemIsDropEnabled;
	}
	TreeItem* item = TreeItemPtr(index);
	Qt::ItemFlags ret = Qt::ItemIsDragEnabled | QAbstractItemModel::flags(index);
	if (item->item_data->GetAreaContent()->GetAreaType() == TITLE_AREA)
	{
		ret |= (Qt::ItemIsEditable | Qt::ItemIsDropEnabled);
	}
	return ret;
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
		TreeItem* move_item = source_parent_item->getChildren()[sourceRow + i];
		
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

		AreaContent* drop_area = drop_item->item_data->GetAreaContent();

		if (row == -1 && column == -1)
		{
			int insert_row = rowCount(parent);
			insertTreeRows(insert_row, parent, drop_area);
		}
		else
		{
			insertTreeRows(row, parent, drop_area);
		}
		delete drag_index;
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
	TreeItem* parent_item = TreeItemPtr(parent);
	if (parent_item->item_data->GetAreaContent()->GetAreaType() != TITLE_AREA)
	{
		return false;
	}
	const QStringList modelTypes = mimeTypes();
	for (int i = 0; i < modelTypes.count(); ++i) {
		if (data->hasFormat(modelTypes.at(i)))
			return true;
	}
	return false;
}

AreaContent* ExportTreeModel::insertTreeRows(int insert_row, const QModelIndex& parent, AreaContent* area_content)
{
	insertRow(insert_row, parent);
	QModelIndex insert_index = index(insert_row, 0, parent);
	TreeItem* t1 = TreeItemPtr(insert_index);
	TreeItem* t2 = TreeItemPtr(parent);
	QMap<int, QVariant> roles = {};
	if (area_content->GetAreaType() == TEXT)
	{
		TextAreaContent* src_content = dynamic_cast<TextAreaContent*>(area_content);
		TextAreaContent* content = new TextAreaContent(*src_content);
		roles[Qt::DisplayRole] = QString::fromLocal8Bit("文本");;
		roles[Qt::TextContentRole] = QVariant::fromValue((void*)content);
		setItemData(insert_index, roles);
		return content;
	}
	else if (area_content->GetAreaType() == TABLE)
	{
		TableAreaContent* src_content = dynamic_cast<TableAreaContent*>(area_content);
		TableAreaContent* content = new TableAreaContent(*src_content);
		roles[Qt::DisplayRole] = QString::fromLocal8Bit("表");
		roles[Qt::TableContentRole] = QVariant::fromValue((void*)content);
		setItemData(insert_index, roles);
		return content;
	}
	else if (area_content->GetAreaType() == IMAGE)
	{
		ImageAreaContent* src_content = dynamic_cast<ImageAreaContent*>(area_content);
		ImageAreaContent* content = new ImageAreaContent(*src_content);
		roles[Qt::DisplayRole] = QString::fromLocal8Bit("图");
		roles[Qt::ImageContentRole] = QVariant::fromValue((void*)content);
		setItemData(insert_index, roles);
		return content;
	}
	else if (area_content->GetAreaType() == LIST)
	{
		ListAreaContent* src_content = dynamic_cast<ListAreaContent*>(area_content);
		ListAreaContent* content = new ListAreaContent(*src_content);
		roles[Qt::DisplayRole] = QString::fromLocal8Bit("清单");
		roles[Qt::ListContentRole] = QVariant::fromValue((void*)content);
		setItemData(insert_index, roles);
		return content;
	}
	else
	{//TITLE_AREA
		TitleAreaContent* src_content = dynamic_cast<TitleAreaContent*>(area_content);
		TitleAreaContent* content = new TitleAreaContent(src_content->title);
		content->title_format = src_content->title_format;
		roles[Qt::EditRole] = content->title;
		roles[Qt::TitleContentRole] = QVariant::fromValue((void*)content);
		setItemData(insert_index, roles);
		int i = 0;
		for (auto it = src_content->GetContentList().cbegin(); it != src_content->GetContentList().cend(); it++, i++)
		{
			insertTreeRows(i, insert_index, (*it));
		}
		return content;
	}
}

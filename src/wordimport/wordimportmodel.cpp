#include "wordimportmodel.h"
#include "qmimedata.h"
#include "qdatastream.h"
#include "global.h"

ImportTreeModel::ImportTreeModel(TreeItem* root, QObject* parent)
	:QUTreeModel(root, parent)
{
}

Qt::ItemFlags ImportTreeModel::flags(const QModelIndex& index) const
{
	return Qt::ItemIsDragEnabled | QAbstractItemModel::flags(index);
}

QMimeData* ImportTreeModel::mimeData(const QModelIndexList& indexes) const
{
	QMimeData* data = new QMimeData();
	QByteArray encoded;
	QDataStream stream(&encoded, QIODevice::WriteOnly);
	QModelIndex* index = new QModelIndex(indexes.at(0));
	stream << (quint64)index;
	data->setData(IMPORT_DROP_STR, encoded);
	return data;
}

ImportTreeModel::~ImportTreeModel()
{
}

#include "qulistmodel.h"

QUListModel::QUListModel(const QVector<QMap<int, QVariant>>& user_data, QObject* parent)
	:QAbstractListModel(parent)
{
	this->user_data = user_data;
}

QVariant QUListModel::data(const QModelIndex& index, int role) const
{
	if (index.isValid()) {
		return user_data[index.row()].value(role, QVariant());
	}
	return QVariant();
}

int QUListModel::rowCount(const QModelIndex& parent) const
{
	return this->user_data.size();
}

void QUListModel::updateData(const QVector<QMap<int, QVariant>>& user_data)
{
	beginResetModel();
	this->user_data = user_data;
	endResetModel();
}




#pragma once
#include "qabstractitemmodel.h"
#include "qobject.h"
#include "qvector.h"
#include "qmap.h"

class QUListModel: public QAbstractListModel
{
	Q_OBJECT
public:
	explicit QUListModel(const QVector<QMap<int, QVariant>>& user_data = {}, QObject* parent = nullptr);
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	void updateData(const QVector<QMap<int, QVariant>>& user_data);

private:
	QVector<QMap<int, QVariant>> user_data;
};


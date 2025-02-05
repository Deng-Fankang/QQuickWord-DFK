#pragma once
#include "qutreemodel.h"

class TitleContentNode;

class ExportTreeModel: public QUTreeModel
{
public:
	explicit ExportTreeModel(TreeItem* root = nullptr, QObject* parent = nullptr);

	bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);

	Qt::ItemFlags flags(const QModelIndex& index) const;

	bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex());

	bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex());

	bool moveRows(const QModelIndex& sourceParent, int sourceRow, int count,
		const QModelIndex& destinationParent, int destinationChild);

	virtual QMimeData* mimeData(const QModelIndexList& indexes) const;
	virtual bool dropMimeData(const QMimeData* data, Qt::DropAction action,
		int row, int column, const QModelIndex& parent);
	virtual Qt::DropActions supportedDropActions() const;
	QStringList mimeTypes() const;

	bool canDropMimeData(const QMimeData* data, Qt::DropAction action,
		int row, int column, const QModelIndex& parent) const;

	void insertTreeRows(int insert_row, const QModelIndex& parent, TitleContentNode* root);
};


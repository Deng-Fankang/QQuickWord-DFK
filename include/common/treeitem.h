#pragma once
#include <memory>
#include "qmap.h"
#include "worddefine.h"
#include "qvariant.h"

using namespace std;

class TreeItem;

class ITreeItemData
{
public:
	ITreeItemData(TreeItem* owner_, AreaContent* area_);

	AreaContent* GetAreaContent() { return ptr_area; }

	virtual ~ITreeItemData();
	void SetParent(TreeItem* dst_parent_, int index = -1);

protected:
	TreeItem* owner;
	AreaContent* ptr_area;
};

class TitleItemData: public ITreeItemData
{
public:
	TitleItemData(TreeItem* owner_, TitleAreaContent* area);
	void RemoveChild(TreeItem* child_);
};


class TextItemData : public ITreeItemData
{
public:
	TextItemData(TreeItem* owner_, TextAreaContent* area);
};

class TableItemData : public ITreeItemData
{
public:
	TableItemData(TreeItem* owner_, TableAreaContent* area);
};


class ImageItemData : public ITreeItemData
{
public:
	ImageItemData(TreeItem* owner_, ImageAreaContent* area);
};

class ListItemData : public ITreeItemData
{
public:
	ListItemData(TreeItem* owner_, ListAreaContent* area);
};


class TreeItem
{
	friend class QUTreeModel;
	friend class ITreeItemData;
public:
	TreeItem(TreeItem* parent_ = nullptr, int index = -1);

	void SetParent(TreeItem* parent_, int index = -1, bool set_item_data=true);
	
	void RemoveChildren(int start, int count);

	const QVector<TreeItem*>& getChildren() { return children; };

	~TreeItem();

	int row();
	QMap<int, QVariant> user_data;
	ITreeItemData* item_data;

private:
	
	QVector<TreeItem*> children;
	TreeItem* parent;
};


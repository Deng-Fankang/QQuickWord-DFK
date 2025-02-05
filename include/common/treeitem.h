#pragma once
#include <memory>
#include "qmap.h"
#include "qvariant.h"

using namespace std;

class TitleContentNode;
class TreeItem;

class ITreeItemData
{
public:
	enum class ItemDataType
	{
		Unknown_Data = 0,
		Title_Data,
	};

	ITreeItemData(TreeItem* owner_) { owner = owner_; data_type = ItemDataType::Unknown_Data; }

	virtual void SetParent(TreeItem* parent_, int index = -1) = 0;

	enum class ItemDataType data_type;
	TreeItem* owner;
};

class TitleItemData: public ITreeItemData
{
public:
	TitleItemData(TreeItem* owner_, TitleContentNode* node);
	void SetParent(TreeItem* dst_parent_, int index = -1) override;
	void SetTitleContentNode(TitleContentNode* node);
	TitleContentNode* GetTitleContentNode();
	~TitleItemData();

private:
	TitleContentNode* ptr_node;
};


class TreeItem
{
public:
	TreeItem(TreeItem* parent_ = nullptr, int index = -1);

	void SetParent(TreeItem* parent_, int index = -1);

	~TreeItem();

	int row();

	ITreeItemData* item_data;
	QMap<int, QVariant> user_data;
	QVector<TreeItem*> children;
	TreeItem* parent;
};


#include "treeitem.h"
#include "worddefine.h"
#include <assert.h>

TitleItemData::TitleItemData(TreeItem* owner_, TitleContentNode* node)
	:ITreeItemData(owner_)
{
	ptr_node = node;
	data_type = ItemDataType::Title_Data;
}

void TitleItemData::SetParent(TreeItem* dst_parent, int index)
{
	assert(dst_parent->item_data->data_type == ItemDataType::Title_Data && owner->parent->item_data->data_type == ItemDataType::Title_Data);
	TitleContentNode* src_parent_node = dynamic_cast<TitleItemData*>(owner->parent->item_data)->ptr_node;
	TitleContentNode* dst_parent_node = dynamic_cast<TitleItemData*>(dst_parent->item_data)->ptr_node;
	
	if (dst_parent_node == src_parent_node)
	{
		const QList<AreaContent*>& src_content_list = src_parent_node->GetContentList();
		int src_index = 0;
		for (; src_index < src_content_list.size(); src_index++)
		{
			if (src_content_list[src_index]->GetAreaType() == TITLE_AREA)
			{
				if (dynamic_cast<TitleAreaContent*>(src_content_list[src_index])->node == ptr_node)
				{
					break;
				}
			}
		}
		if (src_index == index)
		{
			return;
		}
	}
	AreaContent* content = src_parent_node->removeChildTitle(ptr_node);
	ptr_node->setParent(dst_parent_node);

	//可能造成插入的标题与实际不一致
	dst_parent_node->InsertAreaContent(content, index);
}

void TitleItemData::RemoveChild(TreeItem* child_)
{
	assert(child_->item_data->data_type == ItemDataType::Title_Data);
	TitleContentNode* child_node = dynamic_cast<TitleItemData*>(child_->item_data)->ptr_node;
	AreaContent* content = ptr_node->removeChildTitle(child_node);
	delete content;
}

void TitleItemData::SetTitleContentNode(TitleContentNode* node)
{
	ptr_node = node;
}

TitleContentNode* TitleItemData::GetTitleContentNode()
{
	return ptr_node;
}

TitleItemData::~TitleItemData()
{
	if (ptr_node)
	{
		delete ptr_node;
	}
}


TreeItem::TreeItem(TreeItem* parent_, int index)
{
	parent = parent_;
	if (parent) 
	{
		if (index == -1 || index >= parent_->children.size())
		{
			parent_->children.push_back(this);
		}
		else
		{
			parent->children.insert(index, this);
		}
	}
}

void TreeItem::SetParent(TreeItem* parent_, int index)
{
	assert(parent_->item_data->data_type == ITreeItemData::ItemDataType::Title_Data);
	if (parent == parent_ && parent->children.indexOf(this) == index)
	{
		return;
	}
	if (parent)
	{
		parent->children.removeOne(this);
	}
	if (index == -1 || index >= parent_->children.size())
	{
		parent_->children.push_back(this);
	}
	else
	{
		parent_->children.insert(index, this);
	}
	item_data->SetParent(parent_, index);
	parent = parent_;
}

void TreeItem::RemoveChildren(int start, int count)
{
	assert(item_data->data_type == ITreeItemData::ItemDataType::Title_Data);
	int total_count = children.size();
	for (int idx = start; idx < start + count; idx++)
	{
		dynamic_cast<TitleItemData*>(item_data)->RemoveChild(children[idx]);
		delete children[idx];
		if (idx + count < total_count) {
			children[idx] = children[idx + count];
			children[idx + count] = nullptr;
		}
	}
	children.resize(total_count - count);
}


TreeItem::~TreeItem()
{
	for (TreeItem* child : children)
	{
		assert(child != nullptr);
		if (child->item_data)
		{
			delete item_data;
		}
		delete child;
	}
}

int TreeItem::row()
{
	if (parent) {
		for (auto it = parent->children.begin(); it != parent->children.end(); it++) {
			if (*it == this) {
				return it - parent->children.begin();
			}
		}
	}
	return 0;
}


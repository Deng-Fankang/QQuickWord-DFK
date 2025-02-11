#include "treeitem.h"
#include <assert.h>

TreeItem::TreeItem(TreeItem* parent_, int index)
{
	parent = parent_;
	item_data = nullptr;
	if (parent) 
	{
		if (index == -1 || index >= parent->children.size())
		{
			parent->children.push_back(this);
		}
		else
		{
			parent->children.insert(index, this);
		}
	}
}

void TreeItem::SetParent(TreeItem* parent_, int index, bool set_item_data)
{
	assert(parent_->item_data->GetAreaContent()->GetAreaType() == TITLE_AREA);
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
	if (set_item_data)
	{
		item_data->SetParent(parent_, index);
	}
	parent = parent_;
}

void TreeItem::RemoveChildren(int start, int count)
{
	assert(item_data->GetAreaContent()->GetAreaType() == TITLE_AREA);
	int total_count = children.size();
	for (int idx = start; idx < start + count; idx++)
	{
		dynamic_cast<TitleItemData*>(item_data)->RemoveChild(children[idx]);
		delete children[idx];
		children[idx] = nullptr;
		if (idx + count < total_count) {
			children[idx] = children[idx + count];
			children[idx + count] = nullptr;
		}
	}
	for (int idx = start + count; idx < total_count - count; idx++)
	{
		children[idx] = children[idx + count];
		children[idx + count] = nullptr;
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
			delete child->item_data;
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

ITreeItemData::ITreeItemData(TreeItem* owner_,AreaContent* area_)
{
	owner = owner_;
	ptr_area = area_;

}

ITreeItemData::~ITreeItemData()
{
	if (ptr_area)
	{
		delete ptr_area;
		ptr_area = nullptr;
	}
}

void ITreeItemData::SetParent(TreeItem* dst_parent_, int index)
{
	assert(dst_parent_->item_data->GetAreaContent()->GetAreaType() == TITLE_AREA);
	TitleAreaContent* src_parent_area = nullptr;
	if (owner->parent)
	{
		src_parent_area = dynamic_cast<TitleAreaContent*>(dynamic_cast<TitleItemData*>(owner->parent->item_data)->ptr_area);
	}
	TitleAreaContent* dst_parent_area = dynamic_cast<TitleAreaContent*>(dynamic_cast<TitleItemData*>(dst_parent_->item_data)->ptr_area);

	if (src_parent_area == dst_parent_area)
	{
		const QList<AreaContent*>& src_content_list = src_parent_area->GetContentList();
		int src_index = 0;
		for (; src_index < src_content_list.size(); src_index++)
		{
			if (src_content_list[src_index] == ptr_area)
			{
				break;
			}
		}
		if (src_index == index)
		{
			return;
		}
	}
	if(src_parent_area)
	{
		src_parent_area->removeAreaContent(ptr_area);
	}
	ptr_area->setParent(dst_parent_area);

	//可能造成插入的标题与实际不一致
	dst_parent_area->InsertAreaContent(ptr_area, index);
}


TitleItemData::TitleItemData(TreeItem* owner_, TitleAreaContent* area_)
	:ITreeItemData(owner_, area_)
{

}


void TitleItemData::RemoveChild(TreeItem* child_)
{
	AreaContent* child_area = child_->item_data->GetAreaContent();
	dynamic_cast<TitleAreaContent*>(ptr_area)->removeAreaContent(child_area);
}



TextItemData::TextItemData(TreeItem* owner_, TextAreaContent* area)
	:ITreeItemData(owner_, area)
{
}


TableItemData::TableItemData(TreeItem* owner_, TableAreaContent* area)
	:ITreeItemData(owner_, area)
{
}


ImageItemData::ImageItemData(TreeItem* owner_, ImageAreaContent* area)
	:ITreeItemData(owner_, area)
{
}


ListItemData::ListItemData(TreeItem* owner_, ListAreaContent* area)
	:ITreeItemData(owner_, area)
{
}




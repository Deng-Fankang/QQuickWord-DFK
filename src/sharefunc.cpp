#include "sharefunc.h"

TreeItem* GetTitleTreeFromNode(const TitleContentNode* root_title)
{
	if (root_title == nullptr)
	{
		return nullptr;
	}
	TreeItem* root = new TreeItem();
	root->user_data[Qt::DisplayRole] = root_title->title;
	root->user_data[Qt::EditRole] = root_title->title;
	root->item_data = new TitleItemData(root, const_cast<TitleContentNode*>(root_title));
	//root->user_data[Qt::TitleContentRole] = QVariant::fromValue((void*)root_title);
	for (const TitleContentNode* child_title : root_title->GetChildTitleList())
	{
		TreeItem* child = GetTitleTreeFromNode(child_title);
		root->children.push_back(child);
		child->parent = root;
	}
	return root;
}

void DeleteTitleTree(TreeItem* root) {
	if (root == nullptr)
	{
		return;
	}
	if (root->children.size() > 0) {
		for (TreeItem* child_item : root->children) {
			DeleteTitleTree(child_item);
		}
	}
	else
	{
		delete root;
	}
}


TitleContentNode* GetTitleContentNode(TitleContentNode* node, TitleContentNode* parent)
{
	TitleContentNode* ret_node = new TitleContentNode(node->title);
	ret_node->title_format = node->title_format;
	QVector<AreaContent*> content_list;
	for (AreaContent* content : node->GetContentList())
	{
		if (content->GetAreaType() == TEXT)
		{
			TextAreaContent* src_content = dynamic_cast<TextAreaContent*>(content);
			TextAreaContent* content = new TextAreaContent(*src_content);
			ret_node->InsertAreaContent(content);
		}
		else if (content->GetAreaType() == TABLE)
		{
			TableAreaContent* src_content = dynamic_cast<TableAreaContent*>(content);
			TableAreaContent* content = new TableAreaContent(*src_content);
			ret_node->InsertAreaContent(content);
		}
		else if (content->GetAreaType() == IMAGE)
		{
			ImageAreaContent* src_content = dynamic_cast<ImageAreaContent*>(content);
			ImageAreaContent* content = new ImageAreaContent(*src_content);
			ret_node->InsertAreaContent(content);
		}
		else if (content->GetAreaType() == LIST)
		{
			ListAreaContent* src_content = dynamic_cast<ListAreaContent*>(content);
			ListAreaContent* content = new ListAreaContent(*src_content);
			ret_node->InsertAreaContent(content);
		}
		else if (content->GetAreaType() == TITLE_AREA)
		{
			TitleAreaContent* src_content = dynamic_cast<TitleAreaContent*>(content);
			TitleContentNode* node = GetTitleContentNode(src_content->node, ret_node);
			TitleAreaContent* content = new TitleAreaContent(node);
			ret_node->InsertAreaContent(content);
		}
	}
	return ret_node;
}
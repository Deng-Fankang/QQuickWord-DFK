#include "sharefunc.h"

TreeItem* GetTitleTreeFromAreaContent(const AreaContent* area_content)
{
	if (area_content == nullptr)
	{
		return nullptr;
	}
	TreeItem* root = new TreeItem();

	if (area_content->GetAreaType() == TEXT)
	{
		root->user_data[Qt::DisplayRole] = QString::fromLocal8Bit("文本");
		const TextAreaContent* text_area = static_cast<const TextAreaContent*>(area_content);
		root->item_data = new TextItemData(root, const_cast<TextAreaContent*>(text_area));
	}
	else if (area_content->GetAreaType() == IMAGE)
	{
		root->user_data[Qt::DisplayRole] = QString::fromLocal8Bit("图片");
		const ImageAreaContent* image_area = static_cast<const ImageAreaContent*>(area_content);
		root->item_data = new ImageItemData(root, const_cast<ImageAreaContent*>(image_area));
	}
	else if (area_content->GetAreaType() == LIST)
	{
		root->user_data[Qt::DisplayRole] = QString::fromLocal8Bit("清单");
		const ListAreaContent* list_area = static_cast<const ListAreaContent*>(area_content);
		root->item_data = new ListItemData(root, const_cast<ListAreaContent*>(list_area));
	}
	else if (area_content->GetAreaType() == TABLE)
	{
		root->user_data[Qt::DisplayRole] = QString::fromLocal8Bit("表格");
		const TableAreaContent* table_area = static_cast<const TableAreaContent*>(area_content);
		root->item_data = new TableItemData(root, const_cast<TableAreaContent*>(table_area));
	}
	else
	{
		const TitleAreaContent* title_area = static_cast<const TitleAreaContent*>(area_content);
		root->item_data = new TitleItemData(root, const_cast<TitleAreaContent*>(title_area));
		root->user_data[Qt::DisplayRole] = title_area->title;
		for (const AreaContent* child_content : title_area->GetContentList())
		{
			TreeItem* child_item = GetTitleTreeFromAreaContent(child_content);
			child_item->SetParent(root, -1, false);
		}
	}
	return root;
}

void DeleteTitleTree(TreeItem* root) {
	if (root == nullptr)
	{
		return;
	}
	if (root->getChildren().size() > 0) {
		for (TreeItem* child_item : root->getChildren()) {
			DeleteTitleTree(child_item);
		}
	}
	else
	{
		delete root;
	}
}


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


//TitleAreaContent* GetTitleContentNode(TitleAreaContent* node, TitleAreaContent* parent)
//{
//	TitleAreaContent* ret_node = new TitleAreaContent(node->title);
//	ret_node->title_format = node->title_format;
//	QVector<AreaContent*> content_list;
//	for (AreaContent* content : node->GetContentList())
//	{
//		if (content->GetAreaType() == TEXT)
//		{
//			TextAreaContent* src_content = dynamic_cast<TextAreaContent*>(content);
//			TextAreaContent* content = new TextAreaContent(*src_content);
//			ret_node->InsertAreaContent(content);
//		}
//		else if (content->GetAreaType() == TABLE)
//		{
//			TableAreaContent* src_content = dynamic_cast<TableAreaContent*>(content);
//			TableAreaContent* content = new TableAreaContent(*src_content);
//			ret_node->InsertAreaContent(content);
//		}
//		else if (content->GetAreaType() == IMAGE)
//		{
//			ImageAreaContent* src_content = dynamic_cast<ImageAreaContent*>(content);
//			ImageAreaContent* content = new ImageAreaContent(*src_content);
//			ret_node->InsertAreaContent(content);
//		}
//		else if (content->GetAreaType() == LIST)
//		{
//			ListAreaContent* src_content = dynamic_cast<ListAreaContent*>(content);
//			ListAreaContent* content = new ListAreaContent(*src_content);
//			ret_node->InsertAreaContent(content);
//		}
//		else if (content->GetAreaType() == TITLE_AREA)
//		{
//			TitleAreaContent* src_content = dynamic_cast<TitleAreaContent*>(content);
//			TitleAreaContent* node = GetTitleContentNode(src_content->node, ret_node);
//			TitleAreaContent* content = new TitleAreaContent(node);
//			ret_node->InsertAreaContent(content);
//		}
//	}
//	return ret_node;
//}
#include "worddefine.h"
#include "qdebug.h"

TextAreaContent::TextAreaContent(const QString& text, const TextFormat& f)
{
    text_data = text;
    text_format = f;
}

TextAreaContent::TextAreaContent(const TextAreaContent& cp)
{
    text_data = cp.text_data;
    text_format = cp.text_format;
}

void TextAreaContent::PrintContent()
{
    qDebug() << text_data;
}

TableAreaContent::TableAreaContent(const QVector<QVector<CellAreaContent>>& data, const TableFormat& format, QString title)
{
    table_data = data;
    table_format = format;
    this->title = title;
}

TableAreaContent::TableAreaContent(const TableAreaContent& cp)
{
    int nrow = cp.table_data.size();
    int ncol = cp.table_data[0].size();
    if (nrow == 0 || ncol == 0)
    {
        table_data.clear();
    }
    else
    {
        table_data.resize(nrow);
        for (int i = 0; i < nrow; i++)
        {
            table_data[i].resize(ncol);
            for (int j = 0; j < ncol; j++)
            {
                int cell_data_size = cp.table_data[i][j].cell_data.size();
                table_data[i][j].cell_data.resize(cell_data_size);
                for (int k = 0; k < cell_data_size; k++)
                {
                    table_data[i][j].cell_data[k] = cp.table_data[i][j].cell_data[k];
                }
                table_data[i][j].cell_format = cp.table_data[i][j].cell_format;
            }
        }
    }
    title = cp.title;
    table_format = cp.table_format;
}

void TableAreaContent::PrintContent()
{
    int row = table_data.size();
    int col = table_data[0].size();
    for (int i = 0; i < row; i++) {
        qDebug() << endl;
        for (int j = 0; j < col; j++) {
            //qDebug() << table_data[i][j] << ',';
        }
    }
}

TitleAreaContent::TitleAreaContent(TitleContentNode* node_)
{
    node = node_;
}

void TitleAreaContent::PrintContent()
{
    qDebug() << node->title << endl;
}

TitleContentNode::TitleContentNode(const QString& title_)
{
    parent = nullptr;
    level = 0;
    title = title_;
}

TitleContentNode::~TitleContentNode()
{
    for (AreaContent* content : content_list)
    {
        delete content;
    }
}

AreaContent* TitleContentNode::AppendTextAreaContent(QString context, TextFormat f)
{
    TextAreaContent* content = new TextAreaContent(context, f);
    content_list.push_back(content);
    return content;
}

AreaContent* TitleContentNode::AppendTableAreaContent(const QVector<QVector<TableAreaContent::CellAreaContent>>& data, const TableFormat& format, QString area_title)
{
    TableAreaContent* content = new TableAreaContent(data, format, area_title);
    content_list.push_back(content);
    return content;
}

AreaContent* TitleContentNode::AppendListAreaContent(const QString& text, const ListTemplateFormat& list_template_format, const ListFormat& list_format, const TextFormat& text_format)
{
    ListAreaContent* content = new ListAreaContent(list_template_format);
    content_list.push_back(content);
    content->AppendListItem(text, list_format, text_format);
    return content;
}

void TitleContentNode::AppendListItem(const QString& text, const ListFormat& list_format, const TextFormat& text_format)
{
    if (content_list.size() > 0)
    {
        AreaContent* cur_content = content_list.back();
        if (cur_content->GetAreaType() == LIST)
        {
            ListAreaContent* list_area_content = static_cast<ListAreaContent*>(cur_content);
            list_area_content->AppendListItem(text, list_format, text_format);
        }
    }
}

AreaContent* TitleContentNode::AppendImageAreaContent(const QImage& img, ImageFormat f)
{
    ImageAreaContent* content = new ImageAreaContent(img, f);
    content_list.push_back(content);
    return content;
}

AreaContent* TitleContentNode::AppendTitleAreaContent(const QString& title, TextFormat format)
{
    TitleContentNode* child_node = new TitleContentNode(title);
    child_node->title_format = format;
    child_node->parent = this;
    child_node->level = level + 1;
    child_node->index_from_root = index_from_root;
    child_node->index_from_root.push_back(child_title_list.size());
    child_title_list.push_back(child_node);
    TitleAreaContent* content = new TitleAreaContent(child_node);
    content_list.push_back(content);
    return content;
}

void TitleContentNode::InsertAreaContent(AreaContent* content, int insert_idx)
{
    if (insert_idx == -1 || insert_idx >= content_list.size())
    {
        content_list.push_back(content);
    }
    else
    {
        auto it = content_list.begin();
        int idx = 0;
        for (; it != content_list.end(); it++, idx++)
        {
            if (idx == insert_idx)
            {
                content_list.insert(it, content);
                break;
            }
        }
    }
    
    if (content->GetAreaType() == TITLE_AREA)
    {
        TitleAreaContent* title_content = static_cast<TitleAreaContent*>(content);
        title_content->node->parent = this;
        title_content->node->level = level + 1;
        title_content->node->index_from_root = index_from_root;
        title_content->node->index_from_root.push_back(child_title_list.size());
        child_title_list.push_back(title_content->node);
    }
}

TitleContentNode* TitleContentNode::GetParentNode(int level)
{
    if (level >= this->level) {
        return nullptr;
    }
    TitleContentNode* parent_node = parent;
    while (parent_node)
    {
        if (parent_node->level == level) {
            return parent_node;
        }
        parent_node = parent_node->parent;
    }
    return nullptr;
}

TitleContentNode* TitleContentNode::GetChildNode(int index)
{
    return child_title_list.at(index);
}

AreaContent* TitleContentNode::removeChildTitle(TitleContentNode* node)
{
    child_title_list.removeOne(node);
    auto it = content_list.begin();
    for (; it != content_list.end(); it++)
    {
        if ((*it)->GetAreaType() == TITLE_AREA && ((TitleAreaContent*)(*it))->node == node)
        {
            break;
        }
    }
    if (it != content_list.end())
    {
        AreaContent* ret = *it;
        content_list.erase(it);
        return ret;
    }
    
    return nullptr;
}

int TitleContentNode::GetIndex()
{
    return parent->child_title_list.indexOf(this);
}

ListAreaContent::ListAreaContent(const ListAreaContent& cp)
{
    int num = cp.list_data.size();
    list_data.resize(num);
    for (int i = 0; i < num; i++)
    {
        list_data[i].text_content = cp.list_data[i].text_content;
        list_data[i].list_format = cp.list_data[i].list_format;
    }
    list_template_format = cp.list_template_format;
}

ImageAreaContent::ImageAreaContent(const ImageAreaContent& cp)
{
    image = cp.image;
    image_format = cp.image_format;
}

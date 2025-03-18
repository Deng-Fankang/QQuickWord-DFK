#include "worddefine.h"
#include "wordconfig.h"
#include "qdebug.h"

const QVector<QString> HEADING_STYLE_NAME = {
        QString::fromLocal8Bit("标题 1"), QString::fromLocal8Bit("标题 2"),
        QString::fromLocal8Bit("标题 3"), QString::fromLocal8Bit("标题 4"),
        QString::fromLocal8Bit("标题 5"),
};

const TextFormat DEFAULT_TEXT_AREA_FORMAT = {
    {0, 0, 10.5, 0, 0, 0, "等线", "+中文正文", "+西文正文"},
    {12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
    "正文",
};

const TextFormat DEFAULT_TITLE_AREA_FORMAT = {
    {1, 0, 16, 0, 0, 0, "等线", "+中文正文", "+西文正文"},
    {21, 5, 0, 0, 0, 0, 0, 13, 13, 0, 0, 3},
    "标题 3",
};


TextAreaContent::TextAreaContent(const QString& text, const TextFormat& f)
{
    text_data = text;
    text_format = f;
    is_set_format = true;
}

TextAreaContent::TextAreaContent(const QString& text)
{
    text_data = text;
    is_set_format = false;
}

TextAreaContent::TextAreaContent(const TextAreaContent& cp)
{
    text_data = cp.text_data;
    text_format = cp.text_format;
    is_set_format = cp.is_set_format;
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
    is_set_format = true;
}

TableAreaContent::TableAreaContent(const QVector<QVector<QString>>& data)
{
    int row = data.size();
    int col = 0;
    if (row > 0)
    {
        col = data[0].size();
    }
    table_data.resize(row);
    for (int r = 0; r < row; r++)
    {
        table_data[r].resize(col);
        for (int c = 0; c < col; c++)
        {
            table_data[r][c].cell_data[0].text_data = data[r][c];
        }
    }
    is_set_format = false;
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
    is_set_format = cp.is_set_format;
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

void TitleAreaContent::PrintContent()
{
    
}

TitleAreaContent::TitleAreaContent(const QString& title_, TitleAreaContent* parent_area_)
    :AreaContent(parent_area_)
{
    if (parent_area_)
    {
        level = parent_area_->level + 1;
    }
    else
    {
        level = 0;
    }
    
    title = title_;
}

TitleAreaContent::TitleAreaContent(const TitleAreaContent& cp)
{
    if (this == &cp)
    {
        return;
    }
    TitleAreaContent* title_area = CopyTitleContentArea(&cp);
    content_list = std::move(title_area->content_list);
    child_title_list = std::move(title_area->child_title_list);
    level = title_area->level;
    index_from_root = std::move(title_area->index_from_root);
    title = std::move(title_area->title);
    title_format = title_area->title_format;
    list_template_format = title_area->list_template_format;
    is_set_format = title_area->is_set_format;
}

TitleAreaContent::~TitleAreaContent()
{
}

TextAreaContent* TitleAreaContent::AppendTextAreaContent(QString context, TextFormat f)
{
    TextAreaContent* content = new TextAreaContent(context, f);
    content_list.push_back(content);
    return content;
}

TableAreaContent* TitleAreaContent::AppendTableAreaContent(const QVector<QVector<TableAreaContent::CellAreaContent>>& data, const TableFormat& format, QString area_title)
{
    TableAreaContent* content = new TableAreaContent(data, format, area_title);
    content_list.push_back(content);
    return content;
}

ListAreaContent* TitleAreaContent::AppendListAreaContent(const QString& text, const ListTemplateFormat& list_template_format, const ListFormat& list_format, const TextFormat& text_format)
{
    ListAreaContent* content = new ListAreaContent(list_template_format);
    content_list.push_back(content);
    content->AppendListItem(text, list_format, text_format);
    return content;
}

void TitleAreaContent::AppendListItem(const QString& text, const ListFormat& list_format, const TextFormat& text_format)
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

ImageAreaContent* TitleAreaContent::AppendImageAreaContent(const QString& img_path, ImageFormat f)
{
    ImageAreaContent* content = new ImageAreaContent(img_path, f);
    content_list.push_back(content);
    return content;
}

TitleAreaContent* TitleAreaContent::AppendTitleAreaContent(const QString& title, TextFormat format)
{
    TitleAreaContent* child_title_area = new TitleAreaContent(title);
    child_title_area->title_format = format;
    child_title_area->parent_area = this;
    child_title_area->level = level + 1;
    child_title_area->index_from_root = index_from_root;
    child_title_area->index_from_root.push_back(child_title_list.size());
    child_title_list.push_back(child_title_area);
    content_list.push_back(child_title_area);
    return child_title_area;
}

void TitleAreaContent::InsertAreaContent(AreaContent* content, int insert_idx)
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
        TitleAreaContent* title_content = dynamic_cast<TitleAreaContent*>(content);
        title_content->parent_area = this;
        title_content->level = level + 1;
        title_content->index_from_root = index_from_root;
        title_content->index_from_root.push_back(child_title_list.size());
        child_title_list.push_back(title_content);
        if (title_content->child_title_list.size() > 0)
        {
            RecurNumberTitleArea(title_content, title_content->index_from_root, title_content->level);
        }
    }
}

TitleAreaContent* TitleAreaContent::GetParentAreaLevel(int level)
{
    if (level >= this->level) {
        return nullptr;
    }
    TitleAreaContent* parent_res = parent_area;
    while (parent_res)
    {
        if (parent_res->level == level) {
            return parent_res;
        }
        parent_res = parent_res->parent_area;
    }
    return nullptr;
}

TitleAreaContent* TitleAreaContent::GetChildNode(int index)
{
    return child_title_list.at(index);
}

const AreaContent* TitleAreaContent::GetChildContent(int idx) const
{
    if (idx < content_list.size())
    {
        return content_list.at(idx);
    }
    return nullptr;
}


bool TitleAreaContent::removeAreaContent(AreaContent* content)
{
    auto it = content_list.begin();
    for (; it != content_list.end(); it++)
    {
        if ((*it) == content)
        {
            break;
        }
    }
    if (it != content_list.end())
    {
        AreaContent* t = content_list.takeAt(it - content_list.begin());
        return true;
    }
    else
    {
        return false;
    }
}

int TitleAreaContent::GetIndex()
{
    return parent_area->child_title_list.indexOf(this);
}

TitleAreaContent* TitleAreaContent::CopyTitleContentArea(const TitleAreaContent* root, TitleAreaContent* parent)
{
    TitleAreaContent* ret_area = new TitleAreaContent(root->title);
    ret_area->title_format = root->title_format;
    ret_area->list_template_format = root->list_template_format;
    ret_area->is_set_format = root->is_set_format;
    for (AreaContent* content : root->GetContentList())
    {
        if (content->GetAreaType() == TEXT)
        {
            TextAreaContent* src_content = dynamic_cast<TextAreaContent*>(content);
            TextAreaContent* content = new TextAreaContent(*src_content);
            ret_area->InsertAreaContent(content);
        }
        else if (content->GetAreaType() == TABLE)
        {
            TableAreaContent* src_content = dynamic_cast<TableAreaContent*>(content);
            TableAreaContent* content = new TableAreaContent(*src_content);
            ret_area->InsertAreaContent(content);
        }
        else if (content->GetAreaType() == IMAGE)
        {
            ImageAreaContent* src_content = dynamic_cast<ImageAreaContent*>(content);
            ImageAreaContent* content = new ImageAreaContent(*src_content);
            ret_area->InsertAreaContent(content);
        }
        else if (content->GetAreaType() == LIST)
        {
            ListAreaContent* src_content = dynamic_cast<ListAreaContent*>(content);
            ListAreaContent* content = new ListAreaContent(*src_content);
            ret_area->InsertAreaContent(content);
        }
        else if (content->GetAreaType() == TITLE_AREA)
        {
            TitleAreaContent* src_content = dynamic_cast<TitleAreaContent*>(content);
            TitleAreaContent* child_title = CopyTitleContentArea(src_content, ret_area);
            ret_area->InsertAreaContent(child_title);
        }
    }
    return ret_area;
}

void TitleAreaContent::RecurNumberTitleArea(TitleAreaContent* title_area_content, QVector<int> index_from_root, int level)
{
    title_area_content->level = level;
    title_area_content->index_from_root = index_from_root;
    
    for (int i = 0; i < title_area_content->child_title_list.size(); i++)
    {
        index_from_root.push_back(i + 1);
        RecurNumberTitleArea(title_area_content->child_title_list[i], index_from_root, level + 1);
    }
}

void TitleAreaContent::RecurSetConfigFormat(TitleAreaContent* title_area_content)
{
    for (auto it = title_area_content->content_list.begin(); it != title_area_content->content_list.end(); it++)
    {
        if ((*it)->GetAreaType() == TEXT)
        {
            TextAreaContent* text_content = dynamic_cast<TextAreaContent*>(*it);
            WordConfig::GetTextAreaFormat(text_content->text_format);
        }
        else if ((*it)->GetAreaType() == LIST)
        {
            ListAreaContent* list_content = dynamic_cast<ListAreaContent*>(*it);
            TextFormat text_format;
            WordConfig::GetListAreaFormat(list_content->list_template_format, text_format);
            for (int i = 0; i < list_content->list_data.size(); i++)
            {
                list_content->list_data[i].text_content.text_format = text_format;
            }
        }
        else if ((*it)->GetAreaType() == TABLE)
        {
            TableAreaContent* table_content = dynamic_cast<TableAreaContent*>(*it);
            TableFormat table_format;
            TextFormat text_format;
            TableAreaContent::CellFormat cell_format;
            WordConfig::GetTableAreaFormat(table_format, cell_format, text_format);
            int nrow = table_content->table_data.size();
            if (nrow == 0)
            {
                return;
            }
            int ncol = table_content->table_data[0].size();
            for (int row = 0; row < nrow; row++)
            {
                for (int col = 0; col < ncol; col++)
                {
                    QVector<TextAreaContent>& cell_data = table_content->table_data[row][col].cell_data;
                    for (int j = 0; j < cell_data.size(); j++)
                    {
                        cell_data[j].text_format = text_format;
                    }
                    table_content->table_data[row][col].cell_format = cell_format;
                    
                }
            }
        }
        else if ((*it)->GetAreaType() == IMAGE)
        {
            ImageAreaContent* image_content = dynamic_cast<ImageAreaContent*>(*it);
            WordConfig::GetImageAreaFormat(image_content->image_format);
        }
        else
        {
            TitleAreaContent* title_content = dynamic_cast<TitleAreaContent*>(*it);
            WordConfig::GetTitleAreaFormat(title_content->list_template_format, title_content->title_format, title_content->level);
            RecurSetConfigFormat(title_content);
        }
    }
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

void ListAreaContent::AppendListItem(const QString& text, const ListFormat& list_format, const TextFormat& text_format)
{
    ListContent list_content;
    list_content.text_content.text_data = text;
    list_content.text_content.text_format = text_format;
    list_content.list_format = list_format;
    list_data.push_back(list_content);
}

ImageAreaContent::ImageAreaContent(const QString& image_path, const ImageFormat& image_format)
{
    this->image_path = image_path;
    this->image_format = image_format;
    is_set_format = true;
}

ImageAreaContent::ImageAreaContent(const QString& image_path)
{
    this->image_path = image_path;
    is_set_format = false;
}

ImageAreaContent::ImageAreaContent(const ImageAreaContent& cp)
{
    image_path = cp.image_path;
    image_format = cp.image_format;
    is_set_format = cp.is_set_format;
}

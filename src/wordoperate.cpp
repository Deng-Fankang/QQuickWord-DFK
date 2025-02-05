#include "wordoperate.h"
#include "qapplication.h"
#include "qstack.h"
#include "qclipboard.h"
#include "qimage.h"
#include "qmimedata.h"
#include "qdir.h"
#include "qdebug.h"

struct Heading {
    QString style_name;
    WordTitleLevel level;
    int start;
    int end;
};

struct AreaPos {
    int idx;
    int start;
    int end;
    AreaType area_type;
};


void SetTextFormat(QAxObject* text_range, const TextFormat& text_format, bool force_style=true)
{
    if (force_style)
    {
        text_range->dynamicCall("setStyle(const QString&)", text_format.style_name);
    }
    QAxObject* text_font = text_range->querySubObject("Font");
    QAxObject* para_format = text_range->querySubObject("ParagraphFormat");
    text_font->setProperty("Name", text_format.font_name);//设置字体
    text_font->setProperty("Size", text_format.font_size);//设置字体大小
    text_font->setProperty("Bold", text_format.font_bold);
    text_font->setProperty("Italic", text_format.font_italic);
    text_font->setProperty("Spacing", text_format.font_spacing);
    text_font->setProperty("NumberSpacing", text_format.font_number_spacing);
    text_font->setProperty("NameFarEast", text_format.font_name_fareast);
    text_font->setProperty("NameAscii", text_format.font_name_ascii);
    text_font->setProperty("NumberSpacing", text_format.font_number_spacing);
    para_format->setProperty("FirstLineIndent", text_format.first_line_indent);
    para_format->setProperty("LineSpacing", text_format.line_spacing);
    para_format->setProperty("LineSpacingRule", text_format.line_spacing_rule);
    para_format->setProperty("LineUnitAfter", text_format.line_unit_after);
    para_format->setProperty("LineUnitBefore", text_format.line_unit_before);
    para_format->setProperty("LineSpacing", text_format.line_spacing);

    para_format->setProperty("Alignment", text_format.alignment);
    para_format->setProperty("LeftIndent", text_format.left_indent);
    para_format->setProperty("RightIndent", text_format.right_indent);
    para_format->setProperty("SpaceAfter", text_format.spacing_after);
    para_format->setProperty("SpaceBefore", text_format.spacing_before);
    para_format->setProperty("SpaceAfterAuto", text_format.spacing_after_auto);
    para_format->setProperty("SpaceBeforeAuto", text_format.spacing_before_auto);

}


void SetTableFormat(QAxObject* table, const TableFormat& table_format)
{
    table->dynamicCall("setStyle(const QString&)", table_format.style_name);
    //table->querySubObject("Range")->querySubObject("Style")->querySubObject("Table")->setProperty("Alignment", table_format.alignment);
    table->querySubObject("Range")->querySubObject("ParagraphFormat")->setProperty("Alignment", 1);
    table->setProperty("AllowAutoFit", table_format.allow_auto_fit);
    table->setProperty("AllowPageBreaks", table_format.allow_page_break);
    table->setProperty("ApplyStyleFirstColumn", table_format.apply_style_first_col);
    table->setProperty("ApplyStyleHeadingRows", table_format.apply_style_heading);
    table->setProperty("ApplyStyleLastColumn", table_format.apply_style_last_col);
    table->setProperty("ApplyStyleLastRow", table_format.apply_style_last_row);
    table->setProperty("Spacing", table_format.spacing);
    table->setProperty("TableDirection", table_format.table_direction);
    table->setProperty("TopPadding", table_format.top_padding);
    table->setProperty("BottomPadding", table_format.bottom_padding);
    table->setProperty("LeftPadding", table_format.left_padding);
    table->setProperty("RightPadding", table_format.right_padding);
    table->setProperty("LeftIndent", table_format.left_indent);
    //table->querySubObject("Rows")->setProperty("Alignment", table_format.alignment);
}


void SetCellFormat(QAxObject* cell, const TableAreaContent::CellFormat& cell_format)
{
    if (cell_format.height_rule != 0)
    {//不是自动调整行高
        cell->setProperty("Height", cell_format.height);
    }
    cell->setProperty("Width", cell_format.width);
    cell->setProperty("HeightRule", cell_format.height_rule);
    cell->setProperty("PreferredWidth", cell_format.prefer_width);
    cell->setProperty("PreferredWidthType", cell_format.prefer_width_type);
    cell->setProperty("TopPadding", cell_format.top_padding);
    cell->setProperty("BottomPadding", cell_format.bottom_padding);
    cell->setProperty("LeftPadding", cell_format.left_padding);
    cell->setProperty("RightPadding", cell_format.right_padding);
    cell->setProperty("VerticalAlignment", cell_format.ver_alignment);
    cell->setProperty("WordWrap", cell_format.word_wrap);
    cell->setProperty("FitText", cell_format.fit_text);
}


WordReadOperate::WordReadOperate(const QString& file_name)
{
    word = new QAxObject("Word.Application");

    // 设置可见性，若不需要显示Word窗口可以注释掉该行
    word->setProperty("Visible", false);

    // 打开Word文档
    QAxObject* documents = word->querySubObject("Documents");
    doc = documents->querySubObject("Open(const QString&)", file_name);
    heading_style_name = { QString::fromLocal8Bit("标题 1"), QString::fromLocal8Bit("标题 2"), QString::fromLocal8Bit("标题 3") };
}

WordReadOperate::~WordReadOperate()
{
    doc->dynamicCall("Close(bool)", false);
    word->dynamicCall("Quit()");
    delete word;
}

bool AreaPosCompare(const AreaPos& first, const AreaPos& second) 
{
    return first.start < second.start;
}

const QVector<AreaPos> GetAreaInfo(QAxObject* tables, QAxObject* paralists, QAxObject* images)
{
    QVector<AreaPos> area_pos_vector;
    for (int i = 1; i <= tables->property("Count").toInt(); i++)
    {
        QAxObject* table = tables->querySubObject("Item(int)", i);
        AreaPos area_pos;
        area_pos.idx = i;
        area_pos.area_type = TABLE;
        area_pos.start = table->querySubObject("Range")->property("Start").toInt();
        area_pos.end = table->querySubObject("Range")->property("End").toInt();
        area_pos_vector.push_back(area_pos);
    }
    for (int i = 1; i <= paralists->property("Count").toInt(); i++) 
    {
        QAxObject* paralist = paralists->querySubObject("Item(int)", i);
        AreaPos area_pos;
        area_pos.idx = i;
        area_pos.area_type = LIST;
        area_pos.start = paralist->querySubObject("Range")->property("Start").toInt();
        area_pos.end = paralist->querySubObject("Range")->property("End").toInt();
        area_pos_vector.push_back(area_pos);
    }
    for (int i = 1; i <= images->property("Count").toInt(); i++) 
    {
        QAxObject* image = images->querySubObject("Item(int)", i);
        if (image->property("Type") == 3) {
            AreaPos area_pos;
            area_pos.idx = i;
            area_pos.area_type = IMAGE;
            area_pos.start = image->querySubObject("Range")->property("Start").toInt();
            area_pos.end = image->querySubObject("Range")->property("End").toInt();
            area_pos_vector.push_back(area_pos);
        }
    }
    std::sort(area_pos_vector.begin(), area_pos_vector.end(), AreaPosCompare);
    return area_pos_vector;
}

void GetTextAreaInfo(QAxObject* range, QVector<QString>& text_list, QVector<TextFormat>& format_list)
{
    for (int i = 1; i <= range->querySubObject("Paragraphs")->property("Count").toInt(); i++)
    {
        QAxObject* para_range = range->querySubObject("Paragraphs")->querySubObject("Item(int)", i)->querySubObject("Range");
        text_list.push_back(para_range->property("Text").toString().remove('\r'));
        TextFormat format;
        QAxObject* first_char_range = para_range->querySubObject("Characters")->querySubObject("First");
        QAxObject* font = first_char_range->querySubObject("Font");
        QAxObject* para_format = para_range->querySubObject("ParagraphFormat");
        format.font_bold = font->property("Bold").toBool();
        format.font_italic = font->property("Italic").toBool();
        format.font_size = font->property("Size").toFloat();
        format.font_spacing = font->property("Spacing").toFloat();
        format.font_number_spacing = font->property("NumberSpacing").toFloat();
        format.font_name = font->property("Name").toString();
        format.font_name_fareast = font->property("NameFarEast").toString();
        format.font_name_ascii = font->property("NameAscii").toString();
        format.first_line_indent = para_format->property("FirstLineIndent").toFloat();
        format.line_spacing = para_format->property("LineSpacing").toFloat();
        format.line_spacing_rule = para_format->property("LineSpacingRule").toInt();

        format.line_unit_after = para_format->property("LineUnitAfter").toFloat();
        format.line_unit_before = para_format->property("LineUnitBefore").toFloat();
        format.left_indent = para_format->property("LeftIndent").toFloat();
        format.right_indent = para_format->property("RightIndent").toFloat();
        format.spacing_after = para_format->property("SpaceAfter").toFloat();
        format.spacing_before = para_format->property("SpaceBefore").toFloat();
        format.spacing_after_auto = para_format->property("SpaceAfterAuto").toInt();
        format.spacing_before_auto = para_format->property("SpaceBeforeAuto").toInt();
        format.style_name = para_range->querySubObject("Style")->property("NameLocal").toString();
        format.alignment = para_format->property("Alignment").toInt();
        format_list.push_back(format);
    }
}

void AppendContents(TitleContentNode* node, QAxObject* doc, int start, int end)
{
    QAxObject* content_area = doc->querySubObject("Range(int Start, int End)", start, end);
    QAxObject* tables = content_area->querySubObject("Tables");
    QAxObject* paralists = content_area->querySubObject("ListParagraphs");
    QAxObject* images = content_area->querySubObject("InlineShapes");
    const QVector<AreaPos>& area_pos_vector = GetAreaInfo(tables, paralists, images);
    int text_start = start;
    for (int idx = 0; idx < area_pos_vector.size(); idx++) {
        if (text_start < area_pos_vector[idx].start) 
        {
            QVector<QString> text_list;
            QVector<TextFormat> format_list;
            GetTextAreaInfo(doc->querySubObject("Range(int, int)", text_start, area_pos_vector[idx].start), text_list, format_list);
            for (int i = 0; i < text_list.size(); i++) 
            {
                node->AppendTextAreaContent(text_list[i], format_list[i]);
            }
        }
        if (area_pos_vector[idx].area_type == TABLE) {
            QAxObject* table = tables->querySubObject("Item(int)", area_pos_vector[idx].idx);
            int rowCount = table->querySubObject("Rows")->property("Count").toInt();
            int columnCount = table->querySubObject("Columns")->property("Count").toInt();
            QVector<QVector<TableAreaContent::CellAreaContent>> data(rowCount, QVector<TableAreaContent::CellAreaContent>(columnCount));

            TableFormat format;
            format.style_name = table->querySubObject("Style")->property("NameLocal").toString();
            format.alignment = table->querySubObject("Rows")->property("Alignment").toInt();

            format.allow_auto_fit = table->property("AllowAutoFit").toBool();
            format.allow_page_break = table->property("AllowPageBreaks").toBool();
            format.apply_style_first_col = table->property("ApplyStyleFirstColumn").toBool();
            format.apply_style_heading = table->property("ApplyStyleHeadingRows").toBool();
            format.apply_style_last_col = table->property("ApplyStyleLastColumn").toBool();
            format.apply_style_last_row = table->property("ApplyStyleLastRow").toBool();
            format.spacing = table->property("Spacing").toFloat();
            format.table_direction = table->property("TableDirection").toInt();
            format.auto_format_type = table->property("AutoFormatType").toInt();
            format.top_padding = table->property("TopPadding").toFloat();
            format.bottom_padding = table->property("BottomPadding").toFloat();
            format.left_padding = table->property("LeftPadding").toFloat();
            format.right_padding = table->property("RightPadding").toFloat();
            format.left_indent = table->property("LeftIndent").toFloat();

            for (int row = 1; row <= rowCount; row++)
            {
                for (int col = 1; col <= columnCount; col++)
                {
                    TableAreaContent::CellFormat cell_format;
                    QAxObject* cell = table->querySubObject("Cell(int, int)", row, col);
                    QAxObject* cell_range = cell->querySubObject("Range");

                    cell_format.height = cell->property("Height").toFloat();
                    cell_format.width = cell->property("Width").toFloat();
                    cell_format.height_rule = cell->property("HeightRule").toInt();
                    cell_format.prefer_width = cell->property("PreferredWidth").toFloat();
                    cell_format.prefer_width_type = cell->property("PreferredWidthType").toInt();
                    cell_format.top_padding = cell->property("TopPadding").toFloat();
                    cell_format.bottom_padding = cell->property("BottomPadding").toFloat();
                    cell_format.left_padding = cell->property("LeftPadding").toFloat();
                    cell_format.right_padding = cell->property("RightPadding").toFloat();
                    cell_format.ver_alignment = cell->property("VerticalAlignment").toInt();
                    cell_format.word_wrap = cell->property("WordWrap").toBool();
                    cell_format.fit_text = cell->property("FitText").toBool();
                    QVector<QString> text_list;
                    QVector<TextFormat> format_list;
                    GetTextAreaInfo(cell_range, text_list, format_list);
                    data[row - 1][col - 1].cell_data.resize(text_list.size());
                    for (int i = 0; i < text_list.size(); i++)
                    {
                        data[row - 1][col - 1].cell_data[i].text_data = text_list[i];
                        data[row - 1][col - 1].cell_data[i].text_format = format_list[i];
                        data[row - 1][col - 1].cell_format = cell_format;
                    }
                    
                    cell->clear();
                }
            }
            node->AppendTableAreaContent(data, format);
            table->clear();
        }
        else if (area_pos_vector[idx].area_type == LIST) {
            QAxObject* para_range = paralists->querySubObject("Item(int)", area_pos_vector[idx].idx)->querySubObject("Range");
            ListFormat format;
            QAxObject* list_format = para_range->querySubObject("ListFormat");
            format.list_value = list_format->property("ListValue").toInt();
            format.list_level_number = list_format->property("ListLevelNumber").toInt();
            format.list_string = list_format->property("ListString").toString();
            QString name = list_format->querySubObject("List")->property("StyleName").toString();
            bool single_list = list_format->property("SingleList").toBool();
            bool single_list_template = list_format->property("SingleListTemplate").toBool();
            format.list_style_name = para_range->querySubObject("ListStyle")->property("NameLocal").toString();

            QVector<QString> text_list;
            QVector<TextFormat> format_list;
            GetTextAreaInfo(para_range, text_list, format_list);

            QString text = para_range->property("Text").toString().remove('\r');
            if (format.list_value == 1)
            {
                ListTemplateFormat template_format;
                QAxObject* list_template = list_format->querySubObject("ListTemplate");
                template_format.template_name = list_template->property("Name").toString();
                template_format.list_type = list_format->property("ListType").toInt();
                for (int level = 1; level <= 9; level++)
                {
                    QAxObject* list_level = list_template->querySubObject("ListLevels")->querySubObject("Item(int)", level);

                    template_format.list_levels[level - 1].number_format = list_level->property("NumberFormat").toString();
                    template_format.list_levels[level - 1].number_pos = list_level->property("NumberPosition").toFloat();
                    template_format.list_levels[level - 1].number_style = list_level->property("NumberStyle").toInt();
                    template_format.list_levels[level - 1].text_pos = list_level->property("TextPosition").toFloat();
                }
                node->AppendListAreaContent(text, template_format, format, format_list[0]);
            }
            else
            {
                node->AppendListItem(text, format, format_list[0]);
            }
        }
        else if (area_pos_vector[idx].area_type == IMAGE)
        {
            QAxObject* image = images->querySubObject("Item(int)", area_pos_vector[idx].idx);
            image->dynamicCall("Select()");
            QAxObject* sel = doc->querySubObject("Application")->querySubObject("Selection");
            qDebug() << sel->property("Type").toInt() << endl;
            if (sel->property("Type").toInt() == 7) {
                sel->dynamicCall("CopyAsPicture()");
                QClipboard* clipboard = QApplication::clipboard();
                const QMimeData* mimedata = clipboard->mimeData();
                if (mimedata->hasImage()) {
                    QImage image_board = qvariant_cast<QImage>(mimedata->imageData());
                    ImageFormat image_format;
                    image_format.width = image->property("Width").toInt();
                    image_format.height = image->property("Height").toInt();
                    image_format.scale_width = image->property("ScaleWidth").toInt();
                    image_format.scale_height = image->property("ScaleHeight").toInt();
                    QStringList title_index_list;
                    for (int title_index : node->GetIndexFromRoot()) {
                        title_index_list << QString::number(title_index);
                    }
                    image_format.file_name = QString("config/wordimage/%1-%2.png").arg(title_index_list.join('.'), QString::number(area_pos_vector[idx].idx));
                    
                    image_format.file_name = QDir::cleanPath(QDir::currentPath() + QDir::separator() + image_format.file_name);

                    QAxObject* image_range = image->querySubObject("Range");
                    image_format.alignment = image_range->querySubObject("ParagraphFormat")->property("Alignment").toInt();
                    node->AppendImageAreaContent(image_board, image_format);
                    image_board.save(image_format.file_name);
                    //qDebug() << "aliment:" << image->querySubObject("HorizontalLineFormat")->property("Alignment");

                }
            }
        }
        text_start = area_pos_vector[idx].end;
    }
    if (text_start < end) {
        QVector<QString> text_list;
        QVector<TextFormat> format_list;
        GetTextAreaInfo(doc->querySubObject("Range(int, int)", text_start, end), text_list, format_list);
        for (int i = 0; i < text_list.size(); i++) {
            node->AppendTextAreaContent(text_list[i], format_list[i]);
        }
    }
}

TitleContentNode* WordReadOperate::CreateWordNode()
{
    QAxObject* paragraphs = doc->querySubObject("Paragraphs");
    int paragraph_count = paragraphs->property("Count").toInt();
    QVector<Heading> heading_vector;
    QStack<TitleContentNode*> node_stack;
    Heading st;
    st.start = st.end = 0;
    st.level = RootLevel;
    TitleContentNode* root = new TitleContentNode();
    heading_vector.push_back(st);
    node_stack.push(root);
    for (int i = 1; i <= paragraph_count; i++) {
        QAxObject* range = paragraphs->querySubObject("Item(int)", i)->querySubObject("Range");

        int start = range->property("Start").toInt();
        if (start < heading_vector.last().end)
        {
            continue;
        }
        int end = range->property("End").toInt();
        QString style_name = range->querySubObject("Style")->property("NameLocal").toString();
        if (std::find(heading_style_name.begin(), heading_style_name.end(), style_name) != heading_style_name.end()) {
            Heading st;
            st.style_name = style_name;
            st.start = start;
            st.end = end;
            st.level = WordTitleLevel(heading_style_name.indexOf(style_name) + 1);
            TitleContentNode* top_node = node_stack.top();
            int last_end = heading_vector.last().end;
            heading_vector.push_back(st);
            if (top_node->GetTitleLevel() < st.level)
            {
                AppendContents(top_node, doc, last_end, st.start);
                QVector<QString> text_list;
                QVector<TextFormat> format_list;
                GetTextAreaInfo(range, text_list, format_list);
                QString title = range->property("Text").toString().remove('\r');
                TitleAreaContent* title_area = static_cast<TitleAreaContent*>(top_node->AppendTitleAreaContent(title, format_list[0]));
                node_stack.push(title_area->node);
            }
            else
            {
                AppendContents(top_node, doc, last_end, st.start);
                while (top_node->GetTitleLevel() > st.level)
                {
                    node_stack.pop();
                    top_node = node_stack.top();
                }
                //TitleContentNode* node = new TitleContentNode(range->property("Text").toString().remove('\r'), top_node->parent);
                QVector<QString> text_list;
                QVector<TextFormat> format_list;
                GetTextAreaInfo(range, text_list, format_list);
                QString title = range->property("Text").toString().remove('\r');
                TitleAreaContent* title_area = static_cast<TitleAreaContent*>(top_node->GetParent()->AppendTitleAreaContent(title, format_list[0]));
                node_stack.push_back(title_area->node);
            }
        }
    }
    return root;
}

WordWriteOperate::WordWriteOperate(const QString& file_name)
{
    save_name = file_name;
    word = new QAxObject("Word.Application");

    // 设置可见性，若不需要显示Word窗口可以注释掉该行
    word->setProperty("Visible", false);

    // 打开Word文档
    QAxObject* documents = word->querySubObject("Documents");
    doc = documents->querySubObject("Add()");
}

WordWriteOperate::~WordWriteOperate()
{
    doc->dynamicCall("SaveAs(const QString&)", save_name);
    doc->dynamicCall("Close(bool)", false);
    word->dynamicCall("Quit()");
    delete word;
}

void WordWriteOperate::WriteToWord(TitleContentNode* root)
{
    if (root == nullptr) return;
    QAxObject* selection = doc->querySubObject("ActiveWindow")->querySubObject("Selection");
    DoWriteToWord(root, selection);
}

void WordWriteOperate::DoWriteToWord(TitleContentNode* root, QAxObject* selection)
{
    for (auto it = root->GetContentList().constBegin(); it != root->GetContentList().constEnd(); it++)
    {
        if ((*it)->GetAreaType() == TEXT)
        {
            TextAreaContent* text_area = dynamic_cast<TextAreaContent*>(*it);

            SetTextFormat(selection, text_area->text_format);
            selection->dynamicCall("TypeText(const QString&)", text_area->text_data);
            selection->dynamicCall("TypeParagraph(void)");//插入回车
        }
        else if ((*it)->GetAreaType() == TABLE)
        {
            TableAreaContent* table_area = dynamic_cast<TableAreaContent*>(*it);

            int nrow = table_area->table_data.size();
            int ncol = table_area->table_data[0].size();
            QAxObject* pRangef = selection->querySubObject("Range");

            QAxObject* pTablefs = doc->querySubObject("Tables");
            QAxObject* pTablef = pTablefs->querySubObject("Add(QVariant, int, int)", pRangef->asVariant(), nrow, ncol);
            SetTableFormat(pTablef, table_area->table_format);
            for (int row = 1; row <= nrow; row++)
            {
                for (int col = 1; col <= ncol; col++)
                {
                    QAxObject* cell = pTablef->querySubObject("Cell(int, int)", row, col);
                    QAxObject* cell_range = cell->querySubObject("Range");
                    int para_cnt = table_area->table_data[row - 1][col - 1].cell_data.size();

                    SetCellFormat(cell, table_area->table_data[row - 1][col - 1].cell_format);
                    QAxObject* cell_para_range = cell_range;
                    for (int i = 0; i < para_cnt; i++)
                    {
                        QAxObject* cell_para = cell_range->querySubObject("Paragraphs")->querySubObject("Add(QVariant)", cell_para_range->asVariant());
                        const TextAreaContent& text_content = table_area->table_data[row - 1][col - 1].cell_data[i];
                        cell_para->querySubObject("Range")->setProperty("Text", text_content.text_data);
                        cell_para->querySubObject("Range")->dynamicCall("setStyle(WdBuiltinStyle)", "wdStyleNormal");
                        SetTextFormat(cell_para->querySubObject("Range"), text_content.text_format);
                        if (i < para_cnt - 1)
                        {
                            cell_para->querySubObject("Range")->dynamicCall("InsertParagraphAfter()");//插入回车
                        }
                        cell_para_range = cell_para->querySubObject("Range");
                    }
                }
            }
            QVariantList params;
            params.append(6);
            params.append(0);
            selection->dynamicCall("EndOf(QVariant&, QVariant&)", params).toInt();
        }
        else if ((*it)->GetAreaType() == LIST)
        {
            ListAreaContent* list_area = dynamic_cast<ListAreaContent*>(*it);
            int gallery_type = 0;
            if (list_area->list_template_format.list_type == 2)
            {
                gallery_type = 1;
            }
            else if (list_area->list_template_format.list_type == 3)
            {
                gallery_type = 2;
            }
            if (gallery_type == 1 || gallery_type == 2)
            {
                QAxObject* list_template = word->querySubObject("ListGalleries")->querySubObject\
                    ("Item(int)", gallery_type)->querySubObject("ListTemplates")\
                    ->querySubObject("Item(int)", 1);
                QAxObject* list_level = list_template->querySubObject("ListLevels")->querySubObject("Item(int)", 1);
                list_level->setProperty("NumberFormat", list_area->list_template_format.list_levels[0].number_format);
                list_level->setProperty("NumberPosition", list_area->list_template_format.list_levels[0].number_pos);
                list_level->setProperty("NumberStyle", list_area->list_template_format.list_levels[0].number_style);
                list_level->setProperty("TextPosition", list_area->list_template_format.list_levels[0].text_pos);

                QAxObject* pRangef = selection->querySubObject("Range");

                QAxObject* paragraphs = doc->querySubObject("Paragraphs");
                
                selection->querySubObject("Range")->querySubObject("ListFormat")->dynamicCall("ApplyListTemplate(QVariant, QVariant&, QVariant&, QVariant&)", list_template->asVariant(), true, 0, 0);
                for (int i = 0; i < list_area->list_data.size(); i++)
                {
                    SetTextFormat(selection, list_area->list_data[i].text_content.text_format, false);

                    selection->dynamicCall("TypeText(const QString&)", list_area->list_data[i].text_content.text_data);
                    selection->dynamicCall("TypeParagraph(void)");//插入回车
                }
            }
        }
        else if ((*it)->GetAreaType() == IMAGE)
        {
            ImageAreaContent* image_area = dynamic_cast<ImageAreaContent*>(*it);

            QAxObject* inlineShapes = selection->querySubObject("InlineShapes");
            QAxObject* shape = inlineShapes->querySubObject(
                "AddPicture(const QString&)", image_area->image_format.file_name);
            shape->setProperty("Height", image_area->image_format.height);
            shape->setProperty("Width", image_area->image_format.width);
            shape->setProperty("ScaleHeight", image_area->image_format.scale_height);
            shape->setProperty("ScaleWidth", image_area->image_format.scale_width);
            shape->querySubObject("Range")->querySubObject("ParagraphFormat")->setProperty("Alignment", image_area->image_format.alignment);
            //shape->querySubObject("HorizontalLineFormat")->setProperty("PercentWidth", image_area->image_format.percent_width);
            selection->dynamicCall("TypeParagraph(void)");//插入回车
        }
        else if ((*it)->GetAreaType() == TITLE_AREA)
        {
            TitleAreaContent* title_area = dynamic_cast<TitleAreaContent*>(*it);

            SetTextFormat(selection, title_area->node->title_format);

            selection->dynamicCall("TypeText(const QString&)", (title_area->node->title));
            selection->dynamicCall("TypeParagraph(void)");//插入回车
            DoWriteToWord(title_area->node, selection);
        }
    }
}

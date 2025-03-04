#include "wordeditwidget.h"
#include "qabstractitemmodel.h"
#include "qboxlayout.h"
#include "sharemgr.h"
#include "worddefine.h"
#include "qtexttable.h"
#include "qtextlist.h"
#include "qdebug.h"

#include "qgraphicseffect.h"


#define IMAGE_MAX_WIDTH 600

namespace WordEdit {

	void SetTextFormat(const TextFormat& format, QTextBlockFormat& block_format, QTextCharFormat& char_format) {
		QFont font;
		font.setItalic(format.font.font_italic == 1);
		font.setBold(format.font.font_bold == 1);
		font.setPointSize(format.font.font_size);
		font.setFamily(format.font.font_name);
		char_format.setFont(font);
		char_format.setFontWordSpacing(format.font.font_spacing);

		block_format.setTextIndent(format.para_format.first_line_indent);
	}

	WordEditWidget::WordEditWidget(QWidget* parent)
		: BaseFrame(parent)
	{
		ui.setupUi(this);

		tab_widget = new QTabWidget(this);
		import_text_edit = new QTextEdit(this);
		import_text_edit->setLayout(new QVBoxLayout(this));
		import_text_edit->setLineWrapMode(QTextEdit::WidgetWidth);
		import_text_edit->setReadOnly(true);

		export_text_edit = new QTextEdit(this);
		export_text_edit->setLayout(new QVBoxLayout(this));
		export_text_edit->setLineWrapMode(QTextEdit::WidgetWidth);
		export_text_edit->setReadOnly(true);
		tab_widget->addTab(import_text_edit, QString::fromLocal8Bit("导入页"));
		tab_widget->addTab(export_text_edit, QString::fromLocal8Bit("导出页"));
		tab_widget->show();
	
		ui.verticalLayout->addWidget(tab_widget);
		setFrameStyle(QFrame::WinPanel);
		setFrameShadow(QFrame::Sunken);
	}

	WordEditWidget::~WordEditWidget()
	{
		qDebug() << "WordEditWidget destroy" << endl;
	}

	void WordEditWidget::SetImportEditContent(const AreaContent* area_content)
	{
		import_text_edit->clear();
		import_text_edit->setMinimumWidth(300);
		SetRecurEditContent(import_text_edit, area_content);
	}

	void WordEditWidget::SetExportEditContent(const AreaContent* area_content)
	{
		export_text_edit->clear();
		export_text_edit->setMinimumWidth(300);
		SetRecurEditContent(export_text_edit, area_content);
	}

	void WordEditWidget::SetRecurEditContent(QTextEdit* text_edit, const AreaContent* area_content)
	{
		if (area_content->GetAreaType() == TEXT) 
		{
			InsertText(text_edit, dynamic_cast<const TextAreaContent*>(area_content));
		}
		else if (area_content->GetAreaType() == TABLE) 
		{
			InsertTable(text_edit, dynamic_cast<const TableAreaContent*>(area_content));
		}
		else if (area_content->GetAreaType() == LIST)
		{
			InsertList(text_edit, dynamic_cast<const ListAreaContent*>(area_content));
		}
		else if (area_content->GetAreaType() == IMAGE) 
		{
			InsertImage(text_edit, dynamic_cast<const ImageAreaContent*>(area_content));
		}
		else
		{
			const TitleAreaContent* title_content = dynamic_cast<const TitleAreaContent*>(area_content);
			QTextBlockFormat block_format;
			QTextCharFormat char_format;
			SetTextFormat(title_content->title_format, block_format, char_format);
			QTextCursor cursor(text_edit->textCursor());
			cursor.insertBlock(block_format, char_format);
			cursor.insertText(title_content->title);
			for (const AreaContent* content : title_content->GetContentList())
			{
				SetRecurEditContent(text_edit, content);
			}
		}
	}

	void WordEditWidget::InsertText(QTextEdit* text_edit, const TextAreaContent* content)
	{
		QTextCursor cursor(text_edit->textCursor());
		QTextBlockFormat block_format;
		QTextCharFormat char_format;
		SetTextFormat(content->text_format, block_format, char_format);
		cursor.insertBlock(block_format, char_format);
		cursor.insertText(content->text_data);
	}

	void WordEditWidget::InsertTable(QTextEdit* text_edit, const TableAreaContent* content)
	{
		QTextCursor cursor(text_edit->textCursor());
		QTextTableFormat table_format;
		table_format.setAlignment(Qt::AlignCenter);
		const QVector<QVector<TableAreaContent::CellAreaContent>>& data = content->table_data;
		int nrow = data.size();
		int ncol = data[0].size();
		QTextTable* table = cursor.insertTable(nrow, ncol);
		table->setFormat(table_format);
		QTextFrameFormat frame_format = table->frameFormat();
		frame_format.setWidth(QTextLength(QTextLength::PercentageLength, 80));
		table->setFrameFormat(frame_format);
		for (int row = 0; row < nrow; row++) {
			for (int col = 0; col < ncol; col++) {
				QTextTableCell cell = table->cellAt(row, col);
				QTextCursor cellCursor = cell.firstCursorPosition();
				for (int i = 0; i < data[row][col].cell_data.size(); i++)
				{
					cellCursor.insertText(data[row][col].cell_data[i].text_data);
				}
			}
		}
	}

	void WordEditWidget::InsertList(QTextEdit* text_edit, const ListAreaContent* content)
	{
		QTextCursor cursor(text_edit->textCursor());
		QTextBlockFormat block_format;
		QTextCharFormat char_format;

		for (int i = 0; i < content->list_data.size(); i++)
		{
			SetTextFormat(content->list_data[i].text_content.text_format, block_format, char_format);
			if (i == 0)
			{
				QTextListFormat list_format;
				list_format.setStyle(QTextListFormat::ListDecimal);
				if (content->list_template_format.list_type == wdListBullet) {
					list_format.setStyle(QTextListFormat::ListCircle);
				}
				QTextList* text_list = cursor.insertList(list_format);
			}
			else
			{
				cursor.insertBlock();
			}
			cursor.mergeBlockCharFormat(char_format);
			cursor.insertText(content->list_data[i].text_content.text_data, char_format);
		}
	}

	void WordEditWidget::InsertImage(QTextEdit* text_edit, const ImageAreaContent* content)
	{
		QTextCursor cursor(text_edit->textCursor());
		cursor.insertBlock();

		QTextBlockFormat block_format = cursor.blockFormat();
		block_format.setAlignment(Qt::AlignHCenter);
		cursor.mergeBlockFormat(block_format);

		int ori_width = content->image.width();
		int ori_height = content->image.height();
		QImage insert_img = content->image.scaled(min(ori_width, IMAGE_MAX_WIDTH), ori_height, Qt::KeepAspectRatio);
		min_width = qMax(min_width, static_cast<int>(ceil(insert_img.width() + 2 * text_edit->document()->indentWidth())));
		cursor.insertImage(insert_img, content->image_format.file_name);
		text_edit->setMinimumWidth(min_width);
	}
}
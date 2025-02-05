#pragma once

#include <QWidget>
#include "baseframe.h"
#include "qlistwidget.h"
#include "qtextedit.h"
#include "qtablewidget.h"
#include "qpushbutton.h"
#include "ui_wordeditwidget.h"

class TextAreaContent;
class TableAreaContent;
class ListAreaContent;
class ImageAreaContent;
class TitleContentNode;

namespace WordEdit {
	class WordEditWidget : public BaseFrame
	{
		Q_OBJECT

	public:
		WordEditWidget(QWidget* parent = nullptr);
		~WordEditWidget();

		void SetImportEditContent(const TitleContentNode* node);
		void SetExportEditContent(const TitleContentNode* node);

	private:
		void SetRecurEditContent(QTextEdit* text_edit, const TitleContentNode* node);
		void InsertText(QTextEdit* text_edit, const TextAreaContent* content);
		void InsertTable(QTextEdit* text_edit, const TableAreaContent* content);
		void InsertList(QTextEdit* text_edit, const ListAreaContent* content);
		void InsertImage(QTextEdit* text_edit, const ImageAreaContent* content);

	private:
		Ui::WordEditWidgetClass ui;
		QTextEdit* import_text_edit;
		QTextEdit* export_text_edit;
		QTabWidget* tab_widget;
		QPushButton* save_button;
		QPushButton* undo_button;
		int min_width;
	};
}
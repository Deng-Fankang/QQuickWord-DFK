#include "importview.h"
#include "qevent.h"
#include "qmenu.h"
#include "qdir.h"
#include "sharemgr.h"
#include "wordtemplate.h"

FileListView::FileListView(QWidget* parent)
	:QListView(parent)
{
}

void FileListView::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		QListView::mousePressEvent(event);
	}
	else
	{
		operate_index = indexAt(event->pos());
		if (operate_index.isValid())
		{
			ShowMenuPop(event->pos());
		}
	}
}

void FileListView::ShowMenuPop(QPoint pos)
{
	QMenu* menu = new QMenu(this);
	QMenu* tmp_menu = menu->addMenu(QString::fromLocal8Bit("&Æ¥ÅäÄ£°å"));
	QDir tmp_dir("config/wordtemplate");
	QActionGroup* tmp_group = new QActionGroup(menu);
	tmp_group->setExclusive(true);
	for (QString file : tmp_dir.entryList(QDir::Files))
	{
		QAction* action = tmp_group->addAction(file.section('.', 0, 0));
		action->setCheckable(true);
		QString doc_id = WordTemplateMgr::Instance().GetDocId(file);
		bool checked = WordTemplateMgr::Instance().GetWordTemplate(doc_id)->IsMatch();
		action->setChecked(WordTemplateMgr::Instance().GetWordTemplate(doc_id)->IsMatch());
		connect(action, &QAction::triggered, this, [this, doc_id](bool checked) {
			this->TryMatchTemplate(doc_id);
		});
		tmp_menu->addAction(action);
	}
	menu->exec(mapToGlobal(pos));
}

void FileListView::TryMatchTemplate(QString doc_id)
{
	if (operate_index.isValid())
	{
		const ImportFileData& operate_file = ImportFilesMgr::Instance().GetFilesData().at(operate_index.row());
		if (operate_file.root)
		{
			WordTemplate* word_template = WordTemplateMgr::Instance().GetWordTemplate(doc_id);
			word_template->TryMatchAreaContent(operate_file.root);
			if (word_template->IsMatch())
			{
				model()->setData(operate_index, Qt::Checked, Qt::CheckStateRole);
			}
			else
			{
				model()->setData(operate_index, Qt::Unchecked, Qt::CheckStateRole);
			}
		}
	}
}

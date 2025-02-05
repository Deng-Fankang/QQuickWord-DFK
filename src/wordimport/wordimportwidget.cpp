#include "wordimportwidget.h"
#include "qulistmodel.h"
#include "wordimportmodel.h"

#include "module_manager.h"
#include "importwidgetdelegate.h"

#include "qlistview.h"
#include "qtreeview.h"
#include "qsplitter.h"
#include "qfile.h"
#include "qdebug.h"
#include "sharemgr.h"
#include "sharefunc.h"

#include "timertest.h"

namespace WordImport {

	WordImportWidget::WordImportWidget(QWidget* parent)
		: BaseFrame(parent)
	{
		ui.setupUi(this);
		SetWidget();
		connect(&ImportFilesMgr::Instance(), &ImportFilesMgr::importFilesChange, this, &WordImportWidget::ImportFilesChange);
	}

	WordImportWidget::~WordImportWidget()
	{
		qDebug() << "WordImportWidget destroy" << endl;
		Module::InstancePtr()->ClearBuffer();
	}

	void WordImportWidget::SetWidget()
	{
		list_model = new QUListModel();
		tree_model = new ImportTreeModel();
		FileItemDelegate* file_delegate = new FileItemDelegate();
		file_list_view = new QListView(this);
		file_list_view->setModel(list_model);
		file_list_view->setItemDelegate(file_delegate);
		connect(file_list_view->selectionModel(), &QItemSelectionModel::currentChanged, this, &WordImportWidget::OnSelectFile);

		TitleItemDelegate* title_delegate = new TitleItemDelegate();
		title_tree_view = new QTreeView(this);
		title_tree_view->setDragEnabled(true);
		title_tree_view->setModel(tree_model);
		title_tree_view->setItemDelegate(title_delegate);
		title_tree_view->setHeaderHidden(true);
		connect(title_tree_view->selectionModel(), &QItemSelectionModel::currentChanged, this, &WordImportWidget::OnSelectTitle);

		QFile tree_qss_file("config/qss/titletree.qss");
		tree_qss_file.open(QIODevice::ReadOnly);
		title_tree_view->setStyleSheet(tree_qss_file.readAll());
		tree_qss_file.close();

		QSplitter* splitter = new QSplitter(this);
		hbox_layout = new QHBoxLayout();
		hbox_layout->addWidget(splitter);
		splitter->addWidget(file_list_view);
		splitter->addWidget(title_tree_view);
		setLayout(hbox_layout);

		setAttribute(Qt::WA_DeleteOnClose);
		setFrameStyle(QFrame::WinPanel);
		setFrameShadow(QFrame::Sunken);
	}

	void WordImportWidget::OnSelectFile(const QModelIndex& current, const QModelIndex& previous)
	{
		Module::InstancePtr()->m_ui_data.current_file_idx = file_list_view->currentIndex().row();
		tree_model->updateData(Module::InstancePtr()->GetCurFileTitleTree());
	}

	void WordImportWidget::OnSelectTitle(const QModelIndex& current, const QModelIndex& previous)
	{
		TreeItem* tree_item = tree_model->TreeItemPtr(current);
		if (tree_item->item_data->data_type == ITreeItemData::ItemDataType::Title_Data)
		{
			TitleContentNode* node = dynamic_cast<TitleItemData*>(tree_item->item_data)->GetTitleContentNode();
			WordEdit::Module::InstancePtr()->SetImportEditContent(node);
			qDebug() << node->title << endl;
		}
		//TitleContentNode* node = (TitleContentNode*)current.data(Qt::TitleContentRole).value<void*>();
	}


	void WordImportWidget::ImportFilesChange()
	{
		Module::InstancePtr()->ClearBuffer();
		const vector<ImportFileData>& files_data = ImportFilesMgr::Instance().GetFilesData();
		int i = 0;
		for (const ImportFileData& f : files_data) {
			Module::InstancePtr()->file_list_buffer.append({ {Qt::DisplayRole, f.file_name} });
			TreeItem* root = GetTitleTreeFromNode(files_data[i].root);
			Module::InstancePtr()->title_tree_buffer.append(root);
			i++;
		}
		list_model->updateData(Module::InstancePtr()->file_list_buffer);
		file_list_view->setCurrentIndex(list_model->index(0, 0));
	}

	
}
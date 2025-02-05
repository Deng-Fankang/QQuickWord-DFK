#include "wordexportwidget.h"
#include "exportdelegate.h"
#include "worddefine.h"
#include "qfile.h"
#include "sharefunc.h"
#include "module_manager.h"
#include "qdebug.h"

namespace WordExport {
	void TestConstructTree(TreeItem* root) {
		TreeItem* child_1 = new TreeItem(root);
		TreeItem* child_2 = new TreeItem(root);
		child_1->user_data[Qt::EditRole] = QString::fromLocal8Bit("子节点1");
		child_2->user_data[Qt::EditRole] = QString::fromLocal8Bit("子节点2");
		child_1->user_data[Qt::DisplayRole] = QString::fromLocal8Bit("子节点1");
		child_2->user_data[Qt::DisplayRole] = QString::fromLocal8Bit("子节点2");
		child_1->user_data[Qt::UserRole] = TITLE_CONTENT;
		child_2->user_data[Qt::UserRole] = TITLE_CONTENT;

		TreeItem* child_1_1 = new TreeItem(child_1);
		TreeItem* child_1_2 = new TreeItem(child_1);
		child_1_1->user_data[Qt::EditRole] = QString::fromLocal8Bit("子节点11");
		child_1_2->user_data[Qt::EditRole] = QString::fromLocal8Bit("子节点12");
		child_1_1->user_data[Qt::DisplayRole] = QString::fromLocal8Bit("子节点11");
		child_1_2->user_data[Qt::DisplayRole] = QString::fromLocal8Bit("子节点12");
		child_1_1->user_data[Qt::UserRole] = TITLE_CONTENT;
		child_1_2->user_data[Qt::UserRole] = CONTENT;
		TreeItem* child_2_1 = new TreeItem(child_2);
		TreeItem* child_2_2 = new TreeItem(child_2);
		child_2_1->user_data[Qt::EditRole] = QString::fromLocal8Bit("子节点21");
		child_2_2->user_data[Qt::EditRole] = QString::fromLocal8Bit("子节点22");
		child_2_1->user_data[Qt::DisplayRole] = QString::fromLocal8Bit("子节点21");
		child_2_2->user_data[Qt::DisplayRole] = QString::fromLocal8Bit("子节点22");
		child_2_1->user_data[Qt::UserRole] = CONTENT;
		child_2_2->user_data[Qt::UserRole] = TITLE_CONTENT;
	}


	WordExportWidget::WordExportWidget(QWidget* parent)
		: BaseFrame(parent)
	{
		ui.setupUi(this);
		root = new TreeItem();
		root_edit_able = new TreeItem();
		TitleContentNode* root_title_node = new TitleContentNode(QString::fromLocal8Bit("根节点"));
		root_edit_able->user_data[Qt::DisplayRole] = root_title_node->title;
		root_edit_able->user_data[Qt::EditRole] = root_title_node->title;
		//root_edit_able->user_data[Qt::UserRole] = TITLE_CONTENT;
		//root_edit_able->user_data[Qt::TitleContentRole] = QVariant::fromValue((void*)root_title_node);
		root_edit_able->item_data = new TitleItemData(root_edit_able, root_title_node);
		root->children.push_back(root_edit_able);
		//TestConstructTree(root_edit_able);
		SetWidget();
		setFrameStyle(QFrame::WinPanel);
		setFrameShadow(QFrame::Sunken);
	}

	WordExportWidget::~WordExportWidget()
	{
		DeleteTitleTree(root);
		qDebug() << "WordExportWidget destroy" << endl;
	}
	void WordExportWidget::SetWidget()
	{
		title_tree_view = new ExportTitleView(this);
		title_tree_view->setHeaderHidden(true);
		title_tree_view->setDragEnabled(true);
		title_tree_view->setAcceptDrops(true);
		title_tree_model = new ExportTreeModel(root, this);
		//connect(title_tree_model, &ExportTreeModel::dataChanged, this, &WordExportWidget::PrintTree);

		QFile tree_qss_file("config/qss/titletree.qss");
		tree_qss_file.open(QIODevice::ReadOnly);
		title_tree_view->setStyleSheet(tree_qss_file.readAll());
		tree_qss_file.close();

		title_tree_view->setModel(title_tree_model);
		title_tree_view->setItemDelegate(new ExportTitleDelegate());

		connect(title_tree_view->selectionModel(), &QItemSelectionModel::currentChanged, this, &WordExportWidget::OnSelectTitle);
		ui.verticalLayout->addWidget(title_tree_view);
	}

	void WordExportWidget::PrintTree(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles)
	{
		for (TreeItem* child : root_edit_able->children) {
			qDebug() << child->user_data[Qt::DisplayRole].toString() << endl;
		}
		qDebug() << "index:" << topLeft.data().toString() << endl;
	}
	void WordExportWidget::OnSelectTest(const QModelIndex& current, const QModelIndex& previous)
	{
		TreeItem* item = static_cast<TreeItem*>(current.internalPointer());
		qDebug() << "id:" << item << "," << item->user_data[Qt::DisplayRole] << endl;
	}

	void WordExportWidget::OnSelectTitle(const QModelIndex& current, const QModelIndex& previous)
	{
		ITreeItemData* item_data_ptr = GetTreeItemDataPtr(current);
		if (item_data_ptr->data_type == ITreeItemData::ItemDataType::Title_Data)
		{
			TitleContentNode* node = dynamic_cast<TitleItemData*>(item_data_ptr)->GetTitleContentNode();
			if (node == nullptr)
			{
				return;
			}
			WordEdit::Module::InstancePtr()->SetExportEditContent(node);
			qDebug() << node->title << endl;
		}
		
	}
}

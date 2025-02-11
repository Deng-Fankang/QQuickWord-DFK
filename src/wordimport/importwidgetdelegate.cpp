#include "importwidgetdelegate.h"
#include "qboxlayout.h"
#include "qradiobutton.h"
#include "qstyle.h"
#include "qfontmetrics.h"
#include "qdebug.h"
#include "qapplication.h"
#include "qwidget.h"


void FileWidgetHelper::createWidget()
{
    w = new QWidget();
    QVBoxLayout* verticalLayout = new QVBoxLayout(w);
    verticalLayout->setSpacing(6);
    verticalLayout->setContentsMargins(11, 11, 11, 11);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    QRadioButton* radioButton = new QRadioButton(w);
    radioButton->setObjectName("radionButton");
    verticalLayout->addWidget(radioButton);
}

void FileWidgetHelper::setWidgetData(const QModelIndex& index) const
{
    QString s = index.data(Qt::DisplayRole).toString();
    QRadioButton* button = w->findChild<QRadioButton*>("radionButton");
    button->setText(s);
}

void FileItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    painter->save();
    
    QStyleOptionFrame fopt;
    fopt.rect = option.rect;
    fopt.lineWidth = 1;
    fopt.midLineWidth = 3;
    fopt.frameShape = QFrame::Shape::Box;
    fopt.state = QStyle::State_Sunken;
    //fopt.palette.setBrush(QPalette::Mid, QBrush(QColor(Qt::red)));
    //fopt.palette.setBrush(QPalette::Light, QBrush(QColor(Qt::gray)));
    //fopt.palette.setBrush(QPalette::Dark, QBrush(QColor(Qt::gray)));
    //fopt.palette.setColor(QPalette::Button, QColor(Qt::yellow));
    QApplication::style()->drawControl(QStyle::CE_ShapedFrame, &fopt, painter);


    int total_line_width = fopt.lineWidth * 2 + fopt.midLineWidth;
    QStyleOptionViewItem vopt(option);
    vopt.displayAlignment = Qt::AlignVCenter;
    vopt.rect = QRect(option.rect.topLeft() + QPoint(total_line_width, total_line_width), \
        option.rect.bottomRight() - QPoint(total_line_width, total_line_width));
    vopt.text = index.data(Qt::DisplayRole).toString();
    vopt.features |= QStyleOptionViewItem::HasCheckIndicator;
    vopt.checkState = Qt::Checked;
    vopt.palette.setColor(QPalette::Highlight, QColor(0,255,255));
    vopt.palette.setColor(QPalette::HighlightedText, QColor(255, 0, 0));

    if (vopt.state & QStyle::State_Selected) {
        painter->fillRect(vopt.rect, Qt::green);
    }
    else
    {
        painter->fillRect(vopt.rect, Qt::white);
    }
    
    QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &vopt, painter);

    painter->restore();
}

QSize FileItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    QString text = index.data(Qt::DisplayRole).toString();
    QRect rect = option.fontMetrics.boundingRect(option.rect, Qt::TextWordWrap, text);
    size.setHeight(rect.height()*2);
    return size;
}

void ImportTitleDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    TreeTitleItemDelegate::paint(painter, option, index);
}

QSize ImportTitleDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return TreeTitleItemDelegate::sizeHint(option, index);
}

QWidget* ImportTitleDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return TreeTitleItemDelegate::createEditor(parent, option, index);
}

void ImportTitleDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    TreeTitleItemDelegate::setEditorData(editor, index);
}

void ImportTitleDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    TreeTitleItemDelegate::setModelData(editor, model, index);
    //²âÊÔÐÞ¸Ä²Ö¿âÃû
}

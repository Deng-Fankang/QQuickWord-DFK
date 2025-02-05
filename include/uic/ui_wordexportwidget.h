/********************************************************************************
** Form generated from reading UI file 'wordexportwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.11
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WORDEXPORTWIDGET_H
#define UI_WORDEXPORTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WordExportWidgetClass
{
public:
    QVBoxLayout *verticalLayout;

    void setupUi(QWidget *WordExportWidgetClass)
    {
        if (WordExportWidgetClass->objectName().isEmpty())
            WordExportWidgetClass->setObjectName(QString::fromUtf8("WordExportWidgetClass"));
        WordExportWidgetClass->resize(600, 400);
        verticalLayout = new QVBoxLayout(WordExportWidgetClass);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));

        retranslateUi(WordExportWidgetClass);

        QMetaObject::connectSlotsByName(WordExportWidgetClass);
    } // setupUi

    void retranslateUi(QWidget *WordExportWidgetClass)
    {
        WordExportWidgetClass->setWindowTitle(QCoreApplication::translate("WordExportWidgetClass", "WordExportWidget", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WordExportWidgetClass: public Ui_WordExportWidgetClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WORDEXPORTWIDGET_H

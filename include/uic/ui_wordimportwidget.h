/********************************************************************************
** Form generated from reading UI file 'wordimportwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.11
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WORDIMPORTWIDGET_H
#define UI_WORDIMPORTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WordImportWidgetClass
{
public:

    void setupUi(QWidget *WordImportWidgetClass)
    {
        if (WordImportWidgetClass->objectName().isEmpty())
            WordImportWidgetClass->setObjectName(QString::fromUtf8("WordImportWidgetClass"));
        WordImportWidgetClass->resize(600, 400);

        retranslateUi(WordImportWidgetClass);

        QMetaObject::connectSlotsByName(WordImportWidgetClass);
    } // setupUi

    void retranslateUi(QWidget *WordImportWidgetClass)
    {
        WordImportWidgetClass->setWindowTitle(QCoreApplication::translate("WordImportWidgetClass", "WordImportWidget", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WordImportWidgetClass: public Ui_WordImportWidgetClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WORDIMPORTWIDGET_H

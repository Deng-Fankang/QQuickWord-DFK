/********************************************************************************
** Form generated from reading UI file 'wordeditwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.11
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WORDEDITWIDGET_H
#define UI_WORDEDITWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WordEditWidgetClass
{
public:
    QVBoxLayout *verticalLayout;

    void setupUi(QWidget *WordEditWidgetClass)
    {
        if (WordEditWidgetClass->objectName().isEmpty())
            WordEditWidgetClass->setObjectName(QString::fromUtf8("WordEditWidgetClass"));
        WordEditWidgetClass->resize(600, 400);
        verticalLayout = new QVBoxLayout(WordEditWidgetClass);
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);

        retranslateUi(WordEditWidgetClass);

        QMetaObject::connectSlotsByName(WordEditWidgetClass);
    } // setupUi

    void retranslateUi(QWidget *WordEditWidgetClass)
    {
        WordEditWidgetClass->setWindowTitle(QCoreApplication::translate("WordEditWidgetClass", "WordEditWidget", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WordEditWidgetClass: public Ui_WordEditWidgetClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WORDEDITWIDGET_H

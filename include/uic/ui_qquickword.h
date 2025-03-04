/********************************************************************************
** Form generated from reading UI file 'qquickword.ui'
**
** Created by: Qt User Interface Compiler version 5.15.11
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QQUICKWORD_H
#define UI_QQUICKWORD_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QQuickWordClass
{
public:
    QAction *actionimport;
    QAction *actionexport;
    QAction *actionselect;
    QAction *actionclear;
    QAction *actionmatch;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menufile;
    QMenu *menutmplate;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QQuickWordClass)
    {
        if (QQuickWordClass->objectName().isEmpty())
            QQuickWordClass->setObjectName(QString::fromUtf8("QQuickWordClass"));
        QQuickWordClass->resize(941, 436);
        actionimport = new QAction(QQuickWordClass);
        actionimport->setObjectName(QString::fromUtf8("actionimport"));
        actionexport = new QAction(QQuickWordClass);
        actionexport->setObjectName(QString::fromUtf8("actionexport"));
        actionselect = new QAction(QQuickWordClass);
        actionselect->setObjectName(QString::fromUtf8("actionselect"));
        actionclear = new QAction(QQuickWordClass);
        actionclear->setObjectName(QString::fromUtf8("actionclear"));
        actionmatch = new QAction(QQuickWordClass);
        actionmatch->setObjectName(QString::fromUtf8("actionmatch"));
        centralWidget = new QWidget(QQuickWordClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        QQuickWordClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QQuickWordClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 941, 26));
        menufile = new QMenu(menuBar);
        menufile->setObjectName(QString::fromUtf8("menufile"));
        menutmplate = new QMenu(menuBar);
        menutmplate->setObjectName(QString::fromUtf8("menutmplate"));
        QQuickWordClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QQuickWordClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        QQuickWordClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(QQuickWordClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        QQuickWordClass->setStatusBar(statusBar);

        menuBar->addAction(menufile->menuAction());
        menuBar->addAction(menutmplate->menuAction());
        menufile->addAction(actionimport);
        menufile->addAction(actionexport);
        menufile->addAction(actionmatch);
        menufile->addAction(actionclear);
        menutmplate->addAction(actionselect);

        retranslateUi(QQuickWordClass);

        QMetaObject::connectSlotsByName(QQuickWordClass);
    } // setupUi

    void retranslateUi(QMainWindow *QQuickWordClass)
    {
        QQuickWordClass->setWindowTitle(QCoreApplication::translate("QQuickWordClass", "QQuickWord", nullptr));
        actionimport->setText(QCoreApplication::translate("QQuickWordClass", "\345\257\274\345\205\245", nullptr));
        actionexport->setText(QCoreApplication::translate("QQuickWordClass", "\345\257\274\345\207\272", nullptr));
        actionselect->setText(QCoreApplication::translate("QQuickWordClass", "\351\200\211\346\213\251", nullptr));
        actionclear->setText(QCoreApplication::translate("QQuickWordClass", "\346\270\205\351\231\244", nullptr));
        actionmatch->setText(QCoreApplication::translate("QQuickWordClass", "\345\214\271\351\205\215", nullptr));
        menufile->setTitle(QCoreApplication::translate("QQuickWordClass", "\346\226\207\344\273\266", nullptr));
        menutmplate->setTitle(QCoreApplication::translate("QQuickWordClass", "\346\250\241\346\235\277", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QQuickWordClass: public Ui_QQuickWordClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QQUICKWORD_H

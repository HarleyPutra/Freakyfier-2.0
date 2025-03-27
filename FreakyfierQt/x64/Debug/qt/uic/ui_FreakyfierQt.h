/********************************************************************************
** Form generated from reading UI file 'FreakyfierQt.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FREAKYFIERQT_H
#define UI_FREAKYFIERQT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FreakyfierQtClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *FreakyfierQtClass)
    {
        if (FreakyfierQtClass->objectName().isEmpty())
            FreakyfierQtClass->setObjectName("FreakyfierQtClass");
        FreakyfierQtClass->resize(600, 400);
        menuBar = new QMenuBar(FreakyfierQtClass);
        menuBar->setObjectName("menuBar");
        FreakyfierQtClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(FreakyfierQtClass);
        mainToolBar->setObjectName("mainToolBar");
        FreakyfierQtClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(FreakyfierQtClass);
        centralWidget->setObjectName("centralWidget");
        FreakyfierQtClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(FreakyfierQtClass);
        statusBar->setObjectName("statusBar");
        FreakyfierQtClass->setStatusBar(statusBar);

        retranslateUi(FreakyfierQtClass);

        QMetaObject::connectSlotsByName(FreakyfierQtClass);
    } // setupUi

    void retranslateUi(QMainWindow *FreakyfierQtClass)
    {
        FreakyfierQtClass->setWindowTitle(QCoreApplication::translate("FreakyfierQtClass", "FreakyfierQt", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FreakyfierQtClass: public Ui_FreakyfierQtClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FREAKYFIERQT_H

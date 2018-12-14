/********************************************************************************
** Form generated from reading UI file 'clothwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLOTHWINDOW_H
#define UI_CLOTHWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ClothWindow
{
public:
    QWidget *MainWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ClothWindow)
    {
        if (ClothWindow->objectName().isEmpty())
            ClothWindow->setObjectName(QString::fromUtf8("ClothWindow"));
        ClothWindow->resize(400, 300);
        MainWidget = new QWidget(ClothWindow);
        MainWidget->setObjectName(QString::fromUtf8("MainWidget"));
        ClothWindow->setCentralWidget(MainWidget);
        menuBar = new QMenuBar(ClothWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 22));
        ClothWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ClothWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        ClothWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(ClothWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        ClothWindow->setStatusBar(statusBar);

        retranslateUi(ClothWindow);

        QMetaObject::connectSlotsByName(ClothWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ClothWindow)
    {
        ClothWindow->setWindowTitle(QApplication::translate("ClothWindow", "ClothWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ClothWindow: public Ui_ClothWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLOTHWINDOW_H

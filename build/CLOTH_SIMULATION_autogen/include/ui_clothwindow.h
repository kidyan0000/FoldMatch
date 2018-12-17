/********************************************************************************
** Form generated from reading UI file 'clothwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLOTHWINDOW_H
#define UI_CLOTHWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
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
            ClothWindow->setObjectName(QStringLiteral("ClothWindow"));
        ClothWindow->resize(400, 300);
        MainWidget = new QWidget(ClothWindow);
        MainWidget->setObjectName(QStringLiteral("MainWidget"));
        ClothWindow->setCentralWidget(MainWidget);
        menuBar = new QMenuBar(ClothWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 22));
        ClothWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ClothWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        ClothWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(ClothWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        ClothWindow->setStatusBar(statusBar);

        retranslateUi(ClothWindow);

        QMetaObject::connectSlotsByName(ClothWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ClothWindow)
    {
        ClothWindow->setWindowTitle(QApplication::translate("ClothWindow", "ClothWindow", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ClothWindow: public Ui_ClothWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLOTHWINDOW_H

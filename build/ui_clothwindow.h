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
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "cloth_glwidget.h"

QT_BEGIN_NAMESPACE

class Ui_Cloth_Window
{
public:
    QWidget *MainWidget;
    Cloth_GLWidget *cloth_qlwidget;
    QPushButton *pushButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Cloth_Window)
    {
        if (Cloth_Window->objectName().isEmpty())
            Cloth_Window->setObjectName(QString::fromUtf8("Cloth_Window"));
        Cloth_Window->resize(800, 600);
        MainWidget = new QWidget(Cloth_Window);
        MainWidget->setObjectName(QString::fromUtf8("MainWidget"));
        cloth_qlwidget = new Cloth_GLWidget(MainWidget);
        cloth_qlwidget->setObjectName(QString::fromUtf8("cloth_qlwidget"));
        cloth_qlwidget->setGeometry(QRect(20, 20, 500, 500));
        pushButton = new QPushButton(MainWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(570, 20, 89, 25));
        Cloth_Window->setCentralWidget(MainWidget);
        menuBar = new QMenuBar(Cloth_Window);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 800, 22));
        Cloth_Window->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Cloth_Window);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        Cloth_Window->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(Cloth_Window);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        Cloth_Window->setStatusBar(statusBar);

        retranslateUi(Cloth_Window);

        QMetaObject::connectSlotsByName(Cloth_Window);
    } // setupUi

    void retranslateUi(QMainWindow *Cloth_Window)
    {
        Cloth_Window->setWindowTitle(QApplication::translate("Cloth_Window", "ClothWindow", nullptr));
        pushButton->setText(QApplication::translate("Cloth_Window", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Cloth_Window: public Ui_Cloth_Window {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLOTHWINDOW_H

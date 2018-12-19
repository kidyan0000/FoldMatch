/********************************************************************************
** Form generated from reading UI file 'cloth_window.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLOTH_WINDOW_H
#define UI_CLOTH_WINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "cloth_glwidget.h"

QT_BEGIN_NAMESPACE

class Ui_Cloth_Window
{
public:
    QWidget *centralWidget;
    Cloth_GLWidget *widget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Cloth_Window)
    {
        if (Cloth_Window->objectName().isEmpty())
            Cloth_Window->setObjectName(QString::fromUtf8("Cloth_Window"));
        Cloth_Window->resize(800, 600);
        centralWidget = new QWidget(Cloth_Window);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        widget = new Cloth_GLWidget(centralWidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(20, 20, 500, 500));
        Cloth_Window->setCentralWidget(centralWidget);
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
        Cloth_Window->setWindowTitle(QApplication::translate("Cloth_Window", "Cloth_Window", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Cloth_Window: public Ui_Cloth_Window {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLOTH_WINDOW_H

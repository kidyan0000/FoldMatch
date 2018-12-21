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
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "cloth_glwidget.h"

QT_BEGIN_NAMESPACE

class Ui_Cloth_Window
{
public:
    QWidget *central_widget;
    Cloth_GLWidget *cloth_widget;
    QTabWidget *cloth_tab;
    QWidget *tab_3;
    QWidget *tab_4;
    QWidget *tab_5;
    QPushButton *close;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Cloth_Window)
    {
        if (Cloth_Window->objectName().isEmpty())
            Cloth_Window->setObjectName(QString::fromUtf8("Cloth_Window"));
        Cloth_Window->resize(800, 600);
        central_widget = new QWidget(Cloth_Window);
        central_widget->setObjectName(QString::fromUtf8("central_widget"));
        cloth_widget = new Cloth_GLWidget(central_widget);
        cloth_widget->setObjectName(QString::fromUtf8("cloth_widget"));
        cloth_widget->setGeometry(QRect(20, 20, 500, 500));
        cloth_tab = new QTabWidget(central_widget);
        cloth_tab->setObjectName(QString::fromUtf8("cloth_tab"));
        cloth_tab->setGeometry(QRect(550, 20, 200, 500));
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        cloth_tab->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        cloth_tab->addTab(tab_4, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName(QString::fromUtf8("tab_5"));
        cloth_tab->addTab(tab_5, QString());
        close = new QPushButton(central_widget);
        close->setObjectName(QString::fromUtf8("close"));
        close->setGeometry(QRect(749, 0, 50, 20));
        Cloth_Window->setCentralWidget(central_widget);
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
        QObject::connect(close, SIGNAL(clicked()), Cloth_Window, SLOT(close()));

        cloth_tab->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Cloth_Window);
    } // setupUi

    void retranslateUi(QMainWindow *Cloth_Window)
    {
        Cloth_Window->setWindowTitle(QApplication::translate("Cloth_Window", "Cloth_Window", nullptr));
        cloth_tab->setTabText(cloth_tab->indexOf(tab_3), QApplication::translate("Cloth_Window", "Tab 1", nullptr));
        cloth_tab->setTabText(cloth_tab->indexOf(tab_4), QApplication::translate("Cloth_Window", "Tab 2", nullptr));
        cloth_tab->setTabText(cloth_tab->indexOf(tab_5), QApplication::translate("Cloth_Window", "Page", nullptr));
        close->setText(QApplication::translate("Cloth_Window", "close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Cloth_Window: public Ui_Cloth_Window {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLOTH_WINDOW_H

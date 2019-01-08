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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
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
    Cloth_GLWidget *cloth_glwidget;
    QTabWidget *cloth_tab;
    QWidget *tab_1;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *Xrot;
    QSlider *rotXSlider;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *Yrot;
    QSlider *rotYSlider;
    QWidget *horizontalLayoutWidget_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *Zrot;
    QSlider *rotZSlider;
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
        cloth_glwidget = new Cloth_GLWidget(central_widget);
        cloth_glwidget->setObjectName(QString::fromUtf8("cloth_glwidget"));
        cloth_glwidget->setGeometry(QRect(20, 20, 500, 500));
        cloth_tab = new QTabWidget(central_widget);
        cloth_tab->setObjectName(QString::fromUtf8("cloth_tab"));
        cloth_tab->setGeometry(QRect(550, 20, 230, 500));
        tab_1 = new QWidget();
        tab_1->setObjectName(QString::fromUtf8("tab_1"));
        horizontalLayoutWidget = new QWidget(tab_1);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(20, 20, 191, 31));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        Xrot = new QLabel(horizontalLayoutWidget);
        Xrot->setObjectName(QString::fromUtf8("Xrot"));
        Xrot->setEnabled(true);

        horizontalLayout->addWidget(Xrot);

        rotXSlider = new QSlider(horizontalLayoutWidget);
        rotXSlider->setObjectName(QString::fromUtf8("rotXSlider"));
        rotXSlider->setMinimum(-360);
        rotXSlider->setMaximum(360);
        rotXSlider->setSingleStep(16);
        rotXSlider->setPageStep(15);
        rotXSlider->setOrientation(Qt::Horizontal);
        rotXSlider->setTickPosition(QSlider::TicksAbove);
        rotXSlider->setTickInterval(15);

        horizontalLayout->addWidget(rotXSlider);

        horizontalLayoutWidget_2 = new QWidget(tab_1);
        horizontalLayoutWidget_2->setObjectName(QString::fromUtf8("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(20, 60, 191, 31));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        Yrot = new QLabel(horizontalLayoutWidget_2);
        Yrot->setObjectName(QString::fromUtf8("Yrot"));
        Yrot->setEnabled(true);

        horizontalLayout_2->addWidget(Yrot);

        rotYSlider = new QSlider(horizontalLayoutWidget_2);
        rotYSlider->setObjectName(QString::fromUtf8("rotYSlider"));
        rotYSlider->setMinimum(-360);
        rotYSlider->setMaximum(360);
        rotYSlider->setSingleStep(16);
        rotYSlider->setPageStep(15);
        rotYSlider->setOrientation(Qt::Horizontal);
        rotYSlider->setTickPosition(QSlider::TicksAbove);
        rotYSlider->setTickInterval(15);

        horizontalLayout_2->addWidget(rotYSlider);

        horizontalLayoutWidget_3 = new QWidget(tab_1);
        horizontalLayoutWidget_3->setObjectName(QString::fromUtf8("horizontalLayoutWidget_3"));
        horizontalLayoutWidget_3->setGeometry(QRect(20, 100, 191, 31));
        horizontalLayout_3 = new QHBoxLayout(horizontalLayoutWidget_3);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        Zrot = new QLabel(horizontalLayoutWidget_3);
        Zrot->setObjectName(QString::fromUtf8("Zrot"));
        Zrot->setEnabled(true);

        horizontalLayout_3->addWidget(Zrot);

        rotZSlider = new QSlider(horizontalLayoutWidget_3);
        rotZSlider->setObjectName(QString::fromUtf8("rotZSlider"));
        rotZSlider->setMinimum(-360);
        rotZSlider->setMaximum(360);
        rotZSlider->setSingleStep(16);
        rotZSlider->setPageStep(15);
        rotZSlider->setOrientation(Qt::Horizontal);
        rotZSlider->setTickPosition(QSlider::TicksAbove);
        rotZSlider->setTickInterval(15);

        horizontalLayout_3->addWidget(rotZSlider);

        cloth_tab->addTab(tab_1, QString());
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
        QObject::connect(rotXSlider, SIGNAL(valueChanged(int)), cloth_glwidget, SLOT(setXRotation(int)));
        QObject::connect(rotYSlider, SIGNAL(valueChanged(int)), cloth_glwidget, SLOT(setYRotation(int)));
        QObject::connect(rotZSlider, SIGNAL(valueChanged(int)), cloth_glwidget, SLOT(setZRotation(int)));

        cloth_tab->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Cloth_Window);
    } // setupUi

    void retranslateUi(QMainWindow *Cloth_Window)
    {
        Cloth_Window->setWindowTitle(QApplication::translate("Cloth_Window", "Cloth_Window", nullptr));
        Xrot->setText(QApplication::translate("Cloth_Window", "xRot", nullptr));
        Yrot->setText(QApplication::translate("Cloth_Window", "yRot", nullptr));
        Zrot->setText(QApplication::translate("Cloth_Window", "zRot", nullptr));
        cloth_tab->setTabText(cloth_tab->indexOf(tab_1), QApplication::translate("Cloth_Window", "geometry", nullptr));
        close->setText(QApplication::translate("Cloth_Window", "close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Cloth_Window: public Ui_Cloth_Window {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLOTH_WINDOW_H

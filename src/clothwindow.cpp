#include "clothwindow.h"
#include "ui_clothwindow.h"

ClothWindow::ClothWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ClothWindow)
{
    ui->setupUi(this);
}

ClothWindow::~ClothWindow()
{
    delete ui;
}

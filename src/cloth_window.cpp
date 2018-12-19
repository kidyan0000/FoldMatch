#include "cloth_window.h"
#include "ui_cloth_window.h"

Cloth_Window::Cloth_Window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Cloth_Window)
{
    ui->setupUi(this);
}

Cloth_Window::~Cloth_Window()
{
    delete ui;
}

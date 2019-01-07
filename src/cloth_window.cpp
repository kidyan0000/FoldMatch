#include "cloth_window.h"
#include "ui_cloth_window.h"

Cloth_Window::Cloth_Window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Cloth_Window)
{
    ui->setupUi(this);

    connect(ui->cloth_glwidget, SIGNAL(xRotationChanged(int)), ui->rotXSlider, SLOT(setValue(int)));
    connect(ui->cloth_glwidget, SIGNAL(yRotationChanged(int)), ui->rotYSlider, SLOT(setValue(int)));
    connect(ui->cloth_glwidget, SIGNAL(zRotationChanged(int)), ui->rotZSlider, SLOT(setValue(int)));

}

Cloth_Window::~Cloth_Window()
{
    delete ui;
}

void Cloth_Window::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
        close();
    else
        QWidget::keyPressEvent(e);
}

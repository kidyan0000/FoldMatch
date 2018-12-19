#include "cloth_glwidget.h"

#include <QtWidgets>
#include <QtOpenGL>

Cloth_GLWidget::Cloth_GLWidget(QWidget *parent) : QGLWidget(parent)
{

}
void Cloth_GLWidget::initializeGL()
{
    qglClearColor(Qt::black);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void Cloth_GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
}

void Cloth_GLWidget::resizeGL(int w, int h)
{

}

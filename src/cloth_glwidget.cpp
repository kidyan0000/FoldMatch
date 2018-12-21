#include "cloth_glwidget.h"

#include <QtOpenGL>
#include <QtWidgets>
#include <QPainter>

Cloth_GLWidget::Cloth_GLWidget(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{

}

// We define the Frusum of the scene inside the function, The depth channel of the frusum is
// enabled, The CULL Faceb is related to the orientation of the vertices and the faces which can be seen from the
// EYE or CAMERA positions. The GL_SMOOTH is responsible to smooth the color in transitions of the faces sharing common edges
// GL_LIGHTING enabling means the lighting condition is enabled. We can also set mutiple GL_LIGHTING
// e,g LIGHT0 and LIGHT1 etc.
void Cloth_GLWidget::initializeGL()
{
   qglClearColor(Qt::black); //use black backgroud

   glEnable(GL_DEPTH_TEST);
   glEnable(GL_CULL_FACE);
   glShadeModel(GL_SMOOTH);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);

   static GLfloat lightPosition[4] = { 0, 0, 10, 1.0 };
   glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

void Cloth_GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    draw();
}

void Cloth_GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION); //set projection matrix
    glLoadIdentity(); //reload projection matrix

    //what is that?
#ifdef QT_OPENGL_ES_1
    glOrthof(-2, +2, -2, +2, 1.0, 15.0);
#else
    glOrtho(-2, +2, -2, +2, 1.0, 15.0);
#endif
    glMatrixMode(GL_MODELVIEW);
}

void Cloth_GLWidget::draw()
{
    // draw a squard
    qglColor(Qt::red);
    glBegin(GL_QUADS);
        glNormal3f(0,0,-1);
        glVertex3f(-1,-1,0);
        glVertex3f(-1,1,0);
        glVertex3f(1,1,0);
        glVertex3f(1,-1,0);

    glEnd();
    glBegin(GL_TRIANGLES);
        glNormal3f(0,-1,0.707);
        glVertex3f(-1,-1,0);
        glVertex3f(1,-1,0);
        glVertex3f(0,0,1.2);
    glEnd();
    glBegin(GL_TRIANGLES);
        glNormal3f(1,0, 0.707);
        glVertex3f(1,-1,0);
        glVertex3f(1,1,0);
        glVertex3f(0,0,1.2);
    glEnd();
    glBegin(GL_TRIANGLES);
        glNormal3f(0,1,0.707);
        glVertex3f(1,1,0);
        glVertex3f(-1,1,0);
        glVertex3f(0,0,1.2);
    glEnd();
    glBegin(GL_TRIANGLES);
        glNormal3f(-1,0,0.707);
        glVertex3f(-1,1,0);
        glVertex3f(-1,-1,0);
        glVertex3f(0,0,1.2);
    glEnd();
}




#include "cloth_glwidget.h"

#include <QtOpenGL>
#include <QtWidgets>
#include <QPainter>
#include <QGLFunctions>




Cloth_GLWidget::Cloth_GLWidget(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{

}

GLuint Cloth_GLWidget::initVbo()
{
    //
    initializeGLFunctions();

    // get vertices
    ply_module* _plyModule = new ply_module();
    _plyModule->readPLY("../data/Reference-1180330141717.ply", true, true, true, true, true);
    Eigen::MatrixXd verts;
    verts = _plyModule->getVertices();
    // change matrix to row-based vector
    Eigen::MatrixXd verts_t = verts.transpose();
    Eigen::VectorXd verts_row = Eigen::Map<Eigen::VectorXd>(verts_t.data(), verts_t.size());

    // calclate the size of data
    // int verts_size = _plyModule -> getNumberOfVertices();
    int verts_size = verts.size();

    // now we will creat our VBO first we need to ask GL for an opject ID
    GLuint VBOBuffers;

    // now creat the VBO
    glGenBuffers(1, &VBOBuffers);

    // now we bind this ID to an Array buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBOBuffers);

    // finally we stuff our data into the array object
    // first we tell GL it's an array buffer
    // then the number of bytes we are storing (need to tell it's a sizeof FLOAT)
    // then the point to the actual data
    // then how we are going to draw it (in this case statically at the data will not change)
    glBufferData(GL_ARRAY_BUFFER, verts_size*sizeof(GL_FLOAT), &verts_row, GL_STATIC_DRAW);

    // now we can delete our client side datra as we have store it on the GPU
    // now return the VOB data point to our project so we can later for drawing
    return VBOBuffers;
}


// We define the Frusum of the scene inside the function, The depth channel of the frusum is enabled, The CULL Faceb is related to the orientation of the vertices and the faces which can be seen from the EYE or CAMERA positions. The GL_SMOOTH is responsible to smooth the color in transitions of the faces sharing common edges GL_LIGHTING enabling means the lighting condition is enabled. We can also set mutiple GL_LIGHTING e,g LIGHT0 and LIGHT1 etc.
void Cloth_GLWidget::initializeGL()
{
   qglClearColor(Qt::black); //use black backgroud

   initVbo();

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

    draw();
}

void Cloth_GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION); //set projection matrix
    glLoadIdentity(); //reload projection matrix

#ifdef QT_OPENGL_ES_1
    glOrthof(-2, +2, -2, +2, 1.0, 15.0);
#else
    glOrtho(-2, +2, -2, +2, 1.0, 15.0);
#endif
    glMatrixMode(GL_MODELVIEW);
}


void Cloth_GLWidget::draw()
{
    // M1: Immediate Mode
    /*

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
    */

    // M2: use VBO to draw our datas
    // enable vertes
    glEnableClientState(GL_VERTEX_ARRAY);

    // bind our VBO data to be the currently active one
    glBindBuffer(GL_ARRAY_BUFFER, initVbo());

    // tell GL how this data is formated
    // in this case 3 floats tightly packed starting at the beginning of the data
    // (0 = stride, 0 = offset)
    glVertexPointer(3, GL_FLOAT, 0, 0);

    // calcaulate the data size
    ply_module* _plyModule = new ply_module();
    _plyModule->readPLY("../data/Reference-1180330141717.ply", true, true, true, true, true);
    Eigen::MatrixXd verts;
    verts = _plyModule->getVertices();
    int verts_size = verts.size();

    // draw the VBO as a series of GL_LINES starting at 0 in the buffet
    glDrawArrays( GL_TRIANGLES, 0, verts_size);

    // now turn off the VBO client state as we habe finished with it
    glDisableClientState(GL_VERTEX_ARRAY);
}




// #pragma once
#include "cloth_glwidget.h"
#include "cloth_control.h"
#include "cloth_calc.h"

#include <QtOpenGL>
#include <QtWidgets>
#include <QPainter>
#include <QGLFunctions>


Cloth_GLWidget::Cloth_GLWidget(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    xRot = 0;
    yRot = 0;
    zRot = 0;
}

Cloth_GLWidget::~Cloth_GLWidget()
{


}

void Cloth_GLWidget::initVbo()
{

    initializeGLFunctions();

    // get vertices
    ply_module* _plyModule = new ply_module();
    _plyModule->readPLY("../data/Tshirt/Template-1_0001.ply", true, true, true, true, true);


    if (_plyModule->getVertices().rows() != 0)
    {
        verts = _plyModule->getVertices();
    }

    if (_plyModule->getNormals().rows() != 0)
    {
        normals = _plyModule->getNormals();
    }

    if (_plyModule->getColors().rows() != 0)
    {
        colors = _plyModule->getColors();
    }


    // Reshape the matrix to vector

    Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> verts_t(verts);
    Eigen::RowVectorXd verts_row(Eigen::Map<Eigen::RowVectorXd>(verts_t.data(), verts_t.size()));

    Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> normals_t(normals);
    Eigen::RowVectorXd normals_row(Eigen::Map<Eigen::RowVectorXd>(normals_t.data(), normals_t.size()));

    Eigen::Matrix<int,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> colors_t(colors);
    Eigen::RowVectorXi colors_row(Eigen::Map<Eigen::RowVectorXi>(colors_t.data(), colors_t.size()));

    // Eigen::RowVectorXd colorsd_row = colors_row.cast<double>() / 255.;


    // now creat the VBO
    glGenBuffers(1, &VBOBuffers);

    // now we bind this ID to an array buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBOBuffers);


    // then how we are going to draw it (in this case statically at the data will not change)
    glBufferData(GL_ARRAY_BUFFER, (verts.size() + normals.size())*sizeof(double) + (colors.size()*sizeof(int)), 0, GL_STATIC_DRAW);

    // now we copy the data into our subbuffer
    if (_plyModule->getVertices().rows() != 0)
    {
        glBufferSubData(GL_ARRAY_BUFFER, 0, verts.rows()*3*sizeof(double), verts_row.data());
    }
    if (_plyModule->getNormals().rows() != 0)
    {
        glBufferSubData(GL_ARRAY_BUFFER, verts.rows()*3*sizeof(double), normals.rows()*3*sizeof(double), normals_row.data());
    }
    if (_plyModule->getColors().rows() != 0)
    {
        glBufferSubData(GL_ARRAY_BUFFER,(verts.rows()+normals.rows())*3*sizeof(double), colors.rows()*3*sizeof(int), colors_row.data());
    }

    // Unbind our VBO object
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // now creat the index buffer
    /*

    if (_plyModule->getFaces().rows() != 0)
    {
        faces = _plyModule->getFaces();
    }

    Eigen::Matrix<int,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> faces_t(faces);
    Eigen::RowVectorXi faces_row(Eigen::Map<Eigen::RowVectorXi>(faces_t.data(), faces_t.size()));

    glGenBuffers(1, &IndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);

    if (_plyModule->getFaces().rows() != 0)
    {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.rows()*3*sizeof(GL_INT), faces_row.data(), GL_STATIC_DRAW);
    }
    */
}

void Cloth_GLWidget::mapVbo()
{
    /*
    ply_module* _plyModuleCalc = new ply_module();
    _plyModuleCalc->readPLY("../output/debug/lambda1_2.ply", true, true, true, true, true);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (_plyModuleCalc->getVertices().rows() != 0)
    {
        this->vertsCalc = _plyModuleCalc->getVertices();
    }

    if (_plyModuleCalc->getNormals().rows() != 0)
    {
        this->normalsCalc = _plyModuleCalc->getNormals();
    }

    if (_plyModuleCalc->getColors().rows() != 0)
    {
        this->colorsCalc = _plyModuleCalc->getColors();
    }

    // Reshape the matrix to vector

    Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> vertsCalc_t(vertsCalc);
    Eigen::RowVectorXd vertsCalc_row(Eigen::Map<Eigen::RowVectorXd>(vertsCalc_t.data(), vertsCalc_t.size()));

    Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> normalsCalc_t(normalsCalc);
    Eigen::RowVectorXd normalsCalc_row(Eigen::Map<Eigen::RowVectorXd>(normalsCalc_t.data(), normalsCalc_t.size()));

    Eigen::Matrix<int,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> colorsCalc_t(colorsCalc);
    Eigen::RowVectorXi colorsCalc_row(Eigen::Map<Eigen::RowVectorXi>(colorsCalc_t.data(), colorsCalc_t.size()));

    glBindBuffer(GL_ARRAY_BUFFER, VBOBuffers);


    if (_plyModuleCalc->getVertices().rows() != 0)
    {
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertsCalc.rows()*3*sizeof(double), vertsCalc_row.data());
    }
    if (_plyModuleCalc->getNormals().rows() != 0)
    {
        glBufferSubData(GL_ARRAY_BUFFER, vertsCalc.rows()*3*sizeof(double), normalsCalc.rows()*3*sizeof(double), normalsCalc_row.data());
    }
    if (_plyModuleCalc->getColors().rows() != 0)
    {
        glBufferSubData(GL_ARRAY_BUFFER,(vertsCalc.rows()+normalsCalc.rows())*3*sizeof(double), colorsCalc.rows()*3*sizeof(int), colorsCalc_row.data());
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    */
}


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

    initVbo();
    // mapVbo();
}

void Cloth_GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);

    glTranslatef(0.0, 0.0, -6.0);

    glPushMatrix();

    draw();

    glPopMatrix();

    // test();
}

void Cloth_GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);
    // glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION); //set projection matrix
    glLoadIdentity(); //reload projection matrix

#ifdef QT_OPENGL_ES_1
    glOrthof(-2, +2, -2, +2, 1.0, 15.0);
#else
    glOrtho(-1.5, +1.5, -1.5, 1.5, -1.5, 1.5);
#endif
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void Cloth_GLWidget::draw()
{

    #define BUFFER_OFFSET(i) ((double *)NULL + (i))

    // bind our VBO data to be the currently active one
    glBindBuffer(GL_ARRAY_BUFFER, VBOBuffers);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);

    // enable vertex array drawing
    if (verts.rows() != 0)
    {
        glEnableClientState(GL_VERTEX_ARRAY);
    }
    if (normals.rows() != 0)
    {
        glEnableClientState(GL_NORMAL_ARRAY);
    }
    if (colors.rows() != 0)
    {
        glEnableClientState(GL_COLOR_ARRAY);
    }


    // we need to tell GL where the verts start
    if (verts.rows() != 0)
    {
        glVertexPointer(3, GL_DOUBLE, 0, 0);
    }
    if (normals.rows() != 0)
    {
        glNormalPointer(GL_DOUBLE, 0, BUFFER_OFFSET(3*(verts.rows())));

    }
    if (colors.rows() != 0)
    {
        glColorPointer(3, GL_INT, 0, BUFFER_OFFSET(3*(verts.rows() + normals.rows())));

    }

    // draw the VBO as a series of GL_POINTS starting at 0 in the buffet
    glDrawArrays(GL_POINTS, 0, verts.rows());
    // glDrawElements(GL_LINE_STRIP, faces.rows(), GL_UNSIGNED_SHORT, 0);

    // now turn off the VBO client state as we have finished with it
    if (verts.rows() != 0)
    {
        glDisableClientState(GL_VERTEX_ARRAY);
    }
    if (normals.rows() != 0)
    {
        glDisableClientState(GL_NORMAL_ARRAY);
    }
    if (colors.rows() != 0)
    {
        glDisableClientState(GL_COLOR_ARRAY);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

QSize Cloth_GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize Cloth_GLWidget::sizeHint() const
{
    return QSize(400, 400);
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360)
        angle -= 360 * 16;
}

void Cloth_GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void Cloth_GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot + 8 * dy);
        setZRotation(zRot + 8 * dx);
    }

    lastPos = event->pos();
}

void Cloth_GLWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}

void Cloth_GLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}

void Cloth_GLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}

void Cloth_GLWidget::test()
{
    glColor3f(0.5f, 0.5f, 1.0f);
    glPointSize( 2 );

    glBegin(GL_POINTS);
            glVertex3f(0.0f, 0.0f, 5.9f);
            glVertex3f(0.0f, 0.0f, 6.0f);
            glVertex3f(-0.5f, 0.0f, 6.0f);
            glVertex3f(1.0f, 0.0f, 6.0f);
            glVertex3f(0.0f, -0.5f, 6.0f);
            glVertex3f(0.0f, 1.0f, 6.0f);

            glVertex3f(0.0f, 0.0f, 6.5f);
    glEnd();
}


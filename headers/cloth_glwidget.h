#ifndef CLOTH_GLWIDGET_H
#define CLOTH_GLWIDGET_H

#include <QGLWidget>
#include <QtWidgets>
#include <QPainter>
#include <QGLFunctions>

#include <GL/glut.h>

#include <ply_Module.h>

#include <GL/gl.h>

// in order to use <gl> library, we need to inherit the class <QGLFunction>
class Cloth_GLWidget : public QGLWidget, protected QGLFunctions
{
    Q_OBJECT

public:

    explicit Cloth_GLWidget(QWidget *parent = nullptr);
    ~Cloth_GLWidget();

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);	

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

protected:

public slots:
    // slots for xyz-rotation slider
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);

signals:
    // signaling rotation from mouse movement
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);

private:
    // opengl setting
    void initVbo();

    void draw();

    void test();

    // widget setting
    int xRot;
    int yRot;
    int zRot;

    QPoint lastPos;

    // clothing parameters
    Eigen::MatrixXd verts, normals;
    Eigen::MatrixXi colors, faces;

    // opengl parameters
    GLuint VBOBuffers, IndexBuffer;


};

#endif // CLOTH_GLWIDGET_H

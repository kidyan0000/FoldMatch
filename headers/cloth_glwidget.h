#ifndef CLOTH_GLWIDGET_H
#define CLOTH_GLWIDGET_H

#include <QGLWidget>
#include <QtWidgets>
#include <QPainter>
#include <QGLFunctions>

#include <ply_Module.h>

#include <GL/gl.h>

// in order to use <gl> library, we need to inherit the class <QGLFunction>
class Cloth_GLWidget : public QGLWidget, protected QGLFunctions
{
    Q_OBJECT

public:

    explicit Cloth_GLWidget(QWidget *parent = nullptr);

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

protected:

private:

    void initVbo();
    void draw();

    Eigen::MatrixXd verts, normals;
    Eigen::MatrixXi colors;

    // now we will creat our VBO first we need to ask GL for an opject ID
    GLuint VBOBuffers;


};

#endif // CLOTH_GLWIDGET_H

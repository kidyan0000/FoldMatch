#ifndef CLOTH_GLWIDGET_H
#define CLOTH_GLWIDGET_H

#include <QGLWidget>


class Cloth_GLWidget : public QGLWidget
{
    Q_OBJECT

public:

    explicit Cloth_GLWidget(QWidget *parent = nullptr);

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

private:
    void draw();


};

#endif // CLOTH_GLWIDGET_H

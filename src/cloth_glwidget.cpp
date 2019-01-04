#include "cloth_glwidget.h"

#include <QtOpenGL>
#include <QtWidgets>
#include <QPainter>
#include <QGLFunctions>




Cloth_GLWidget::Cloth_GLWidget(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{

}

void Cloth_GLWidget::initVbo()
{

    initializeGLFunctions();

    // get vertices
    ply_module* _plyModule = new ply_module();
    _plyModule->readPLY("../data/Template-1_0001.ply", true, true, true, true, true);

    //normals = (_plyModule->getNormals().rows() != 0) ? _plyModule->getNormals(): 0 ;

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
        normals = _plyModule->getNormals();

    }

    // Reshape the matrix to vector
    // https://eigen.tuxfamily.org/dox/group__TutorialReshapeSlicing.html

    Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> verts_t(verts);
    Eigen::Map<Eigen::RowVectorXd> verts(verts_t.data(), verts_t.size());
    // Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> normals_t(normals);
    // Eigen::Map<Eigen::RowVectorXd> normals(normals_t.data(), normals_t.size());
    Eigen::Matrix<int,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> colors_t(colors);
    Eigen::Map<Eigen::RowVectorXi> colors(colors_t.data(), colors_t.size());

    std::cout << &verts << std::endl;

    // now creat the VBO
    glGenBuffers(1, &VBOBuffers);

    // now we bind this ID to an Array buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBOBuffers);


    // finally we stuff our data into the array object
    // first we tell GL it's an array buffer
    // then the number of bytes we are storing (need to tell it's a sizeof FLOAT)
    // then the point to the actual data
    // then how we are going to draw it (in this case statically at the data will not change)
    glBufferData(GL_ARRAY_BUFFER, verts.size()*2*sizeof(GL_DOUBLE), 0, GL_STATIC_DRAW);
    // now we copy the data for the verts into our buffer first
    glBufferSubData(GL_ARRAY_BUFFER,0,verts.rows()*3*sizeof(GL_DOUBLE),verts.data());
    // now we need to tag the normals onto the end of the verts
    // glBufferSubData(GL_ARRAY_BUFFER,data_num*3*sizeof(GL_FLOAT),data_num*3*sizeof(GL_FLOAT),normals_row.data());
    // now we need to tag the colours onto the end of the normals
    glBufferSubData(GL_ARRAY_BUFFER,colors.rows()*3*sizeof(GL_DOUBLE),colors.rows()*3*sizeof(GL_DOUBLE),colors.data());

    // now we can delete our client side datra as we have store it on the GPU
    // now return the VOB data point to our project so we can later for drawing
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
    glOrtho(-2, +2, -2, +2, -2, 2);
#endif
    glMatrixMode(GL_MODELVIEW);
}


void Cloth_GLWidget::draw()
{

    #define BUFFER_OFFSET(i) ((double *)NULL + (i))

    glPushMatrix();

    static int xrot=0;
    static int yrot=0;
    glRotatef(xrot++,1,0,0);
    glRotatef(yrot++,0,1,0);

    // enable vertex array drawing
    glEnableClientState(GL_VERTEX_ARRAY);
    // enable Normal array
    glEnableClientState(GL_NORMAL_ARRAY);
    // enable the colour array
    glEnableClientState(GL_COLOR_ARRAY);

    // bind our VBO data to be the currently active one
    glBindBuffer(GL_ARRAY_BUFFER, VBOBuffers);

    // tell GL how this data is formated
    // in this case 3 floats tightly packed starting at the beginning of the data
    // (0 = stride, 0 = offset)
    // we need to tell GL where the verts start
    glVertexPointer(3,GL_DOUBLE,0,0);
    // now we tell it where the nornals are (thes are basically at the end of the verts
    // glNormalPointer(GL_FLOAT, 0, BUFFER_OFFSET(3*25125));
    // now we tell it where the colours are (thes are basically at the end of the normals
    glColorPointer(3,GL_DOUBLE, 0, BUFFER_OFFSET(3*25125));


    // draw the VBO as a series of GL_LINES starting at 0 in the buffet
    glDrawArrays(GL_TRIANGLE_STRIP,0,25125);

    // now turn off the VBO client state as we have finished with it
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    glPopMatrix();

}




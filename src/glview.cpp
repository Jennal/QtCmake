#include "glview.h"
#include <QtGui/QResizeEvent>
#include "qtlogo.h"

GLView::GLView(QWidget *parent, const int width, const int height)
    : QGLWidget(QGLFormat(QGL::SampleBuffers | QGL::DoubleBuffer), parent)
    , mouseMoveFunc(NULL)
    , mousePressFunc(NULL)
    , mouseReleaseFunc(NULL)
    , frameResizeFunc(NULL)
    , keyEventFunc(NULL)
{
    resize(width, height);

    logo = 0;
    xRot = 0;
    yRot = 0;
    zRot = 0;

    qtGreen = QColor::fromCmykF(0.40, 0.0, 1.0, 0.0);
    qtPurple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);
}

GLView::~GLView()
{
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

//! [5]
void GLView::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
//        swapBuffers();
    }
}
//! [5]

void GLView::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
//        swapBuffers();
    }
}

void GLView::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
//        swapBuffers();
    }
}

//! [6]
void GLView::initializeGL()
{
    qglClearColor(qtPurple.dark());

    logo = new QtLogo(this, 64);
    logo->setColor(qtGreen.dark());

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
//    glEnable(GL_LIGHTING);
//    glEnable(GL_LIGHT0);
    glEnable(GL_MULTISAMPLE);
//    static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
//    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}
//! [6]

//! [7]
void GLView::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);
//    logo->draw();
    glBegin(GL_TRIANGLES);
      glColor3f(1.0, 0.0, 0.0);
      glVertex3f(0, 0.25, 0);
      glVertex3f(-0.25, -0.25, 0);
      glVertex3f(0.25, -0.25, 0);
    glEnd();
}
//! [7]

//! [8]
void GLView::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#ifdef QT_OPENGL_ES_1
    glOrthof(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
#else
    glOrtho(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
#endif
    glMatrixMode(GL_MODELVIEW);
}
//! [8]

void GLView::setMouseMoveFunc(MOUSEEVENT_FUNCPTR func)
{
    mouseMoveFunc = func;
}

void GLView::setMousePressFunc(MOUSEEVENT_FUNCPTR func)
{
    mousePressFunc = func;
}

void GLView::setMouseReleaseFunc(MOUSEEVENT_FUNCPTR func)
{
    mouseReleaseFunc = func;
}

void
GLView::setResizeFunc(RESIZE_FUNCPTR func)
{
    frameResizeFunc = func;
}

void GLView::setKeyEventFunc(ACCEL_FUNCPTR func)
{
    keyEventFunc = func;
}

void GLView::mouseMoveEvent(QMouseEvent *event)
{
    QGLWidget::mouseMoveEvent(event);
    //CCTRACE();
    if (mouseMoveFunc)
        mouseMoveFunc(event);

    //sample
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

void GLView::mousePressEvent(QMouseEvent *event)
{
    QGLWidget::mousePressEvent(event);
    //CCTRACE();
    if (mousePressFunc)
        mousePressFunc(event);

    //sample
    lastPos = event->pos();
}

void GLView::mouseReleaseEvent(QMouseEvent *event)
{
    QGLWidget::mouseReleaseEvent(event);
    //CCTRACE();
    if (mouseReleaseFunc)
        mouseReleaseFunc(event);
}

void GLView::keyPressEvent(QKeyEvent *e)
{
    QGLWidget::keyPressEvent(e);
    //CCTRACE();
    if (keyEventFunc)
        keyEventFunc(e);
}

void GLView::keyReleaseEvent(QKeyEvent *e)
{
    QGLWidget::keyReleaseEvent(e);
    //CCTRACE();
    if (keyEventFunc)
        keyEventFunc(e);
}

void
GLView::resizeEvent(QResizeEvent *event)
{
    QGLWidget::resizeEvent(event);
    //CCTRACE();
    if (frameResizeFunc)
        frameResizeFunc(event);
}


#ifndef  _GL_WIDGET_H_
#define  _GL_WIDGET_H_

#include <QtOpenGL/QGLWidget>

class QtLogo;
class QTimer;

typedef void(*MOUSEEVENT_FUNCPTR)(QMouseEvent *event);
typedef void(*RESIZE_FUNCPTR)(QResizeEvent* event);
typedef void(*ACCEL_FUNCPTR)(QKeyEvent *event);

class GLView : public QGLWidget
{
    Q_OBJECT

public:
    explicit GLView(QWidget *parent = 0, const int width = 800, const int height = 600);
    virtual ~GLView();

    void setMouseMoveFunc( MOUSEEVENT_FUNCPTR func);
    void setMousePressFunc( MOUSEEVENT_FUNCPTR func);
    void setMouseReleaseFunc( MOUSEEVENT_FUNCPTR func);
    void setResizeFunc( RESIZE_FUNCPTR func);
    void setKeyEventFunc( ACCEL_FUNCPTR func);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    MOUSEEVENT_FUNCPTR mouseMoveFunc;
    MOUSEEVENT_FUNCPTR mousePressFunc;
    MOUSEEVENT_FUNCPTR mouseReleaseFunc;
    RESIZE_FUNCPTR frameResizeFunc;

    ACCEL_FUNCPTR keyEventFunc;

//! [1]
public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);

signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);
//! [1]

private:
    QtLogo *logo;
    int xRot;
    int yRot;
    int zRot;
    QPoint lastPos;
    QColor qtGreen;
    QColor qtPurple;
};

#endif // _GL_WIDGET_H_

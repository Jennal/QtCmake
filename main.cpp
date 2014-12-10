#include "mainwindow.h"
#include "application.h"
#include "glview.h"
#include <QtGui/QMouseEvent>

static Application* application = nullptr;
static GLView* glView = nullptr;
static QPoint lastPos;
static int xRot = 0;
static int yRot = 0;
static int zRot = 0;

static void qNormalizeAngle(int &src)
{
    int angle = src;
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;

    if (angle != src)
        src = angle;
}

static void mouse_press(QMouseEvent *event)
{
    if( ! glView) return;

    lastPos = event->pos();
}

static void mouse_move(QMouseEvent *event)
{
    if( ! glView) return;

    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        xRot += 8 * dy;
        yRot += 8 * dx;
        qNormalizeAngle(xRot);
        qNormalizeAngle(yRot);
    } else if (event->buttons() & Qt::RightButton) {
        xRot += 8 * dy;
        zRot += 8 * dx;
        qNormalizeAngle(xRot);
        qNormalizeAngle(zRot);
    }

    application->setRotate(xRot, yRot, zRot);

    lastPos = event->pos();
}

int main(int argc, char** argv)
{
    Application app(argc, argv);
    application = &app;

	MainWindow mainWin;
    auto size = mainWin.getGLWidget()->size();
    GLView view(mainWin.getGLWidget());

    glView = &view;
    Q_UNUSED(size);

    view.setMousePressFunc(&mouse_press);
    view.setMouseMoveFunc(&mouse_move);

	mainWin.show();

    return app.run();
}

#include "application.h"
#include <algorithm>
#include "qtlogo.h"
#include <QtCore/QTimer>

// sharedApplication pointer
Application * Application::sm_pSharedApplication = 0;

Application::Application(int argc, char *argv[])
    : QApplication(argc, argv)
    , m_timer(NULL)
    , m_refCount(0)
{
    m_nAnimationInterval = 1.0f / 60.0f * 1000.0f;
    Q_ASSERT(! sm_pSharedApplication);
    sm_pSharedApplication = this;
}

Application::~Application()
{
    Q_ASSERT(this == sm_pSharedApplication);
    sm_pSharedApplication = NULL;
}

void Application::setGLView(GLView* glview)
{
    Q_ASSERT( ! m_glView && ! logo);
    m_glView = glview;
    logo = new QtLogo(this, 64);
}

void Application::setRotate(int x, int y, int z)
{
    xRot = x;
    yRot = y;
    zRot = z;
}

int Application::run()
{
    // Initialize instance and cocos2d.
//    if (!applicationDidFinishLaunching())
//    {
//        return 0;
//    }

    m_timer = new QTimer(this);
    QObject::connect(m_timer, SIGNAL(timeout()),
                     this, SLOT(timerUpdate()));
    m_timer->start(m_nAnimationInterval);

    return Application::getInstance()->exec();
}

void Application::setAnimationInterval(double interval)
{
    m_nAnimationInterval = interval * 1000.0f;
    if (m_timer)
    {
        m_timer->start(m_nAnimationInterval);
    }
}

//////////////////////////////////////////////////////////////////////////
// static member function
//////////////////////////////////////////////////////////////////////////
Application* Application::getInstance()
{
    Q_ASSERT(sm_pSharedApplication);
    return sm_pSharedApplication;
}

void Application::timerUpdate()
{
    // m_refCount is here to prevent calling the mainloop from nested event loops
    if (!m_refCount)
    {
//        Director::getInstance()->mainLoop();
//        printf("x, y, z = (%d, %d, %d)\n", xRot, yRot, zRot);
//        fflush(stdout);
//        m_glView->makeCurrent();
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        glLoadIdentity();
//        glTranslatef(0.0, 0.0, -10.0);
//        glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
//        glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
//        glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);
//        logo->draw();
//        m_glView->updateGL();
//        m_glView->doneCurrent();
    }
}

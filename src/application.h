#ifndef __CC_APPLICATION_WIN32_H__
#define __CC_APPLICATION_WIN32_H__

#include <string>

#undef CursorShape
#include <QtWidgets/QApplication>
#include "glview.h"

//class CCRect;

class Application : public QApplication
{
    Q_OBJECT
public:
    Application(int argc, char *argv[]);
    virtual ~Application();

    /**
    @brief    Run the message loop.
    */
    int run();

    /**
    @brief    Get current applicaiton instance.
    @return Current application instance pointer.
    */
    static Application* getInstance();

    /* override functions */
    virtual void setAnimationInterval(double interval);

    void setGLView(GLView*);
    void setRotate(int x, int y, int z);
public Q_SLOTS:
    void timerUpdate();

protected:
    long long           m_nAnimationInterval;
    std::string         m_startupScriptFilename;

    static Application * sm_pSharedApplication;

private:
    QTimer *m_timer;
    int m_refCount;

    QtLogo *logo;
    int xRot;
    int yRot;
    int zRot;
    QPoint lastPos;
    GLView* m_glView;
};

#endif    // __CC_APPLICATION_WIN32_H__

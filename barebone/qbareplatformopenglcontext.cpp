#include "qbareplatformopenglcontext.h"

#include <QOpenGLContext>
#include <qpa/qplatformopenglcontext.h>

QBarePlatformOpenGLContext::QBarePlatformOpenGLContext(const QSurfaceFormat &format, QPlatformOpenGLContext *share, EGLDisplay display)
{

}

void QBarePlatformOpenGLContext::swapBuffers(QPlatformSurface *surface)
{
}

bool QBarePlatformOpenGLContext::makeCurrent(QPlatformSurface *surface)
{
}

void QBarePlatformOpenGLContext::doneCurrent()
{
}

QFunctionPointer QBarePlatformOpenGLContext::getProcAddress(const QByteArray &procName)
{
	return 0;
}

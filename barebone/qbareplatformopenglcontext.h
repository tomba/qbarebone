#ifndef QBAREPLATFORMOPENGLCONTEXT_H
#define QBAREPLATFORMOPENGLCONTEXT_H

#include <QOpenGLContext>
#include <qpa/qplatformopenglcontext.h>

typedef void *EGLDisplay;

class QBarePlatformOpenGLContext : public QPlatformOpenGLContext
{
public:
	QBarePlatformOpenGLContext(const QSurfaceFormat &format, QPlatformOpenGLContext *share, EGLDisplay display);

	// QPlatformOpenGLContext interface
public:
	QSurfaceFormat format() const { return m_format; }
	void swapBuffers(QPlatformSurface *surface);
	bool makeCurrent(QPlatformSurface *surface);
	void doneCurrent();
	QFunctionPointer getProcAddress(const QByteArray &procName);

private:
	QSurfaceFormat m_format;
};

#endif // QBAREPLATFORMOPENGLCONTEXT_H

#include "qbareintegration.h"
#include "qbarebackingstore.h"
#include "qbarescreen.h"
#include "qbarewindow.h"
#include "qbareclientinterface.h"
#include "qbareplatformopenglcontext.h"

#include <QOffscreenSurface>

#include <QtGui/private/qpixmap_raster_p.h>
#include <QtGui/private/qguiapplication_p.h>
#include <qpa/qplatformwindow.h>
#include <qpa/qplatformfontdatabase.h>
#include <qpa/qplatforminputcontext.h>
#include <qpa/qplatformoffscreensurface.h>

#include <qpa/qplatforminputcontextfactory_p.h>

#include <QtPlatformSupport/private/qgenericunixeventdispatcher_p.h>
#include <QtPlatformSupport/private/qevdevkeyboardmanager_p.h>
#include <QtPlatformSupport/private/qevdevmousemanager_p.h>
#include <QtPlatformSupport/private/qeglpbuffer_p.h>

#include <cstdio>

QT_BEGIN_NAMESPACE

QBareIntegration::QBareIntegration(const QStringList &parameters)
{
	Q_UNUSED(parameters);

	printf("QBareIntegration()\n");

	m_nativeInterface = new QBareNativeInterface(this);

}

QBareIntegration::~QBareIntegration()
{
}

void QBareIntegration::initialize()
{
	printf("INIT\n");
	m_inputContext = QPlatformInputContextFactory::create();

	//createInputHandlers();
}

void QBareIntegration::createInputHandlers()
{
	//m_kbdMgr = new QEvdevKeyboardManager(QLatin1String("EvdevKeyboard"), QString() /* spec */);
	m_mouseMgr = new QEvdevMouseManager(QLatin1String("EvdevMouse"), QString() /* spec */);

#if 0
	Q_FOREACH (QScreen *screen, QGuiApplication::screens()) {
		QEGLPlatformCursor *cursor = static_cast<QEGLPlatformCursor *>(screen->handle()->cursor());
		if (cursor)
			cursor->setMouseDeviceDiscovery(mouseMgr->deviceDiscovery());
	}
	new QEvdevTouchScreenHandlerThread(QString() /* spec */, this);
#endif
}

void QBareIntegration::test()
{
	printf("TESTI\n");
	m_clientInterface->test();
}

void QBareIntegration::add_screen(QSize size)
{
	QBareScreen *screen = new QBareScreen(QRect(0, 0, size.width(), size.height()), 32, QImage::Format_ARGB32_Premultiplied);
	screenAdded(screen);
}

void QBareIntegration::install_client(QBareClientInterface* client)
{
	m_clientInterface = client;
}

bool QBareIntegration::hasCapability(QPlatformIntegration::Capability cap) const
{
	switch (cap) {
	case ThreadedPixmaps: return true;

	//case MultipleWindows: return false;
	//case WindowManagement: return false;

	case ApplicationState: return true;
	case NativeWidgets: return true;
	case OpenGL: return true;
	case ForeignWindows: return true;
	//case ThreadedOpenGL: return !needsBasicRenderloopWorkaround();
	case RasterGLSurface: return true;

	default: return QPlatformIntegration::hasCapability(cap);
	}
}

QPlatformFontDatabase *QBareIntegration::fontDatabase() const
{
	return QPlatformIntegration::fontDatabase();
}

QPlatformWindow *QBareIntegration::createPlatformWindow(QWindow *window) const
{
	QBareWindow *w = new QBareWindow(window);
	w->requestActivateWindow();
	return w;
}

QPlatformBackingStore *QBareIntegration::createPlatformBackingStore(QWindow *window) const
{
	return new QBareBackingStore(window, (QBareIntegration*)this);
}

QPlatformOpenGLContext *QBareIntegration::createPlatformOpenGLContext(QOpenGLContext *context) const
{
	QSurfaceFormat format(context->format());
	format.setAlphaBufferSize(8);
	format.setRedBufferSize(8);
	format.setGreenBufferSize(8);
	format.setBlueBufferSize(8);
	return new QBarePlatformOpenGLContext(format, context->shareHandle(), m_eglDisplay);
}

QPlatformOffscreenSurface *QBareIntegration::createPlatformOffscreenSurface(QOffscreenSurface *surface) const
{
	QSurfaceFormat format(surface->requestedFormat());
	format.setAlphaBufferSize(8);
	format.setRedBufferSize(8);
	format.setGreenBufferSize(8);
	format.setBlueBufferSize(8);

	return new QEGLPbuffer(m_eglDisplay, format, surface);
}

QAbstractEventDispatcher *QBareIntegration::createEventDispatcher() const
{
	return createUnixEventDispatcher();
}

QBareIntegration *QBareIntegration::instance()
{
	return static_cast<QBareIntegration *>(QGuiApplicationPrivate::platformIntegration());
}

QPlatformNativeInterface *QBareIntegration::nativeInterface() const
{
	printf("GET NATIVE\n");
	return m_nativeInterface;
}

QPlatformServices *QBareIntegration::services() const
{
	printf("GET SERVICE\n");
	return 0;
}

QT_END_NAMESPACE

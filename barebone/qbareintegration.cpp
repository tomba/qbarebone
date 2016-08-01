#include "qbareintegration.h"
#include "qbarebackingstore.h"
#include "qbarescreen.h"
#include "qbarewindow.h"

#include <QtGui/private/qpixmap_raster_p.h>
#include <QtGui/private/qguiapplication_p.h>
#include <qpa/qplatformwindow.h>
#include <qpa/qplatformfontdatabase.h>
#include <qpa/qplatforminputcontext.h>

#include <qpa/qplatforminputcontextfactory_p.h>

#include <QtPlatformSupport/private/qgenericunixeventdispatcher_p.h>
#include <QtPlatformSupport/private/qevdevkeyboardmanager_p.h>
#include <QtPlatformSupport/private/qevdevmousemanager_p.h>
#include <QtPlatformSupport/private/qgenericunixfontdatabase_p.h>

#include <cstdio>

QBareIntegration::QBareIntegration(const QStringList &parameters)
{
	Q_UNUSED(parameters);

	printf("QBareIntegration()\n");

	m_nativeInterface = new QBareNativeInterface(this);

	m_fontDatabase.reset(new QGenericUnixFontDatabase());
}

QBareIntegration::~QBareIntegration()
{
}

void QBareIntegration::initialize()
{
	m_inputContext = QPlatformInputContextFactory::create();

	//createInputHandlers();
}

void QBareIntegration::createInputHandlers()
{
	//m_kbdMgr = new QEvdevKeyboardManager(QLatin1String("EvdevKeyboard"), QString() /* spec */);
	//m_mouseMgr = new QEvdevMouseManager(QLatin1String("EvdevMouse"), QString() /* spec */);

#if 0
	Q_FOREACH (QScreen *screen, QGuiApplication::screens()) {
		QEGLPlatformCursor *cursor = static_cast<QEGLPlatformCursor *>(screen->handle()->cursor());
		if (cursor)
			cursor->setMouseDeviceDiscovery(mouseMgr->deviceDiscovery());
	}
	new QEvdevTouchScreenHandlerThread(QString() /* spec */, this);
#endif
}

QBareScreenInterface* QBareIntegration::add_screen(QSize size, QString name)
{
	QBareScreen *screen = new QBareScreen(name, QRect(0, 0, size.width(), size.height()), 32,
					      QImage::Format_ARGB32_Premultiplied,
					      (QBareIntegration*)this);
	screenAdded(screen);
	return screen;
}

void QBareIntegration::install_client(QBareClientInterface* client)
{
	m_clientInterface = client;
}


bool QBareIntegration::hasCapability(QPlatformIntegration::Capability cap) const
{
	switch (cap) {
	case ThreadedPixmaps: return true;
	case MultipleWindows: return false;
	case NonFullScreenWindows: return true;
	case WindowManagement: return false;
	case OpenGL: return false;
	default: return QPlatformIntegration::hasCapability(cap);
	}
}

QPlatformFontDatabase *QBareIntegration::fontDatabase() const
{
	//return QPlatformIntegration::fontDatabase();
	return m_fontDatabase.data();
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
	//printf("GET NATIVE\n");
	return m_nativeInterface;
}

QPlatformServices *QBareIntegration::services() const
{
	//printf("GET SERVICE\n");
	return 0;
}

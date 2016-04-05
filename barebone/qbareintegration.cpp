#include "qbareintegration.h"
#include "qbarebackingstore.h"
#include "qbarescreen.h"
#include "qbarewindow.h"
#include "qbareclientinterface.h"

#include <QtGui/private/qpixmap_raster_p.h>
#include <QtGui/private/qguiapplication_p.h>
#include <qpa/qplatformwindow.h>
#include <qpa/qplatformfontdatabase.h>

#include <QtPlatformSupport/private/qgenericunixeventdispatcher_p.h>

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
	case MultipleWindows: return false;
	case WindowManagement: return false;
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

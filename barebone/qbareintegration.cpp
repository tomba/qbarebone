#include "qbareintegration.h"
#include "qbarebackingstore.h"
#include "qbarescreen.h"
#include "qbarewindow.h"

#include <QtGui/private/qpixmap_raster_p.h>
#include <QtGui/private/qguiapplication_p.h>
#include <qpa/qplatformwindow.h>
#include <qpa/qplatformfontdatabase.h>

#include <QtPlatformSupport/private/qgenericunixeventdispatcher_p.h>

#include <cstdio>

QT_BEGIN_NAMESPACE

static const char debugBackingStoreEnvironmentVariable[] = "QT_DEBUG_BACKINGSTORE";

static inline unsigned parseOptions(const QStringList &paramList)
{
	unsigned options = 0;
	foreach (const QString &param, paramList) {
		if (param == QLatin1String("enable_fonts"))
			options |= QBareIntegration::EnableFonts;
	}
	return options;
}

QBareIntegration::QBareIntegration(const QStringList &parameters)
	: m_dummyFontDatabase(0)
	, m_options(parseOptions(parameters))
{
	printf("QBareIntegration()\n");

	m_nativeInterface = new QBareNativeInterface(this);

	if (qEnvironmentVariableIsSet(debugBackingStoreEnvironmentVariable)
			&& qgetenv(debugBackingStoreEnvironmentVariable).toInt() > 0) {
		m_options |= DebugBackingStore | EnableFonts;
	}

}

QBareIntegration::~QBareIntegration()
{
	delete m_dummyFontDatabase;
}

void QBareIntegration::add_screen(QSize size)
{
	QBareScreen *screen = new QBareScreen(QRect(0, 0, size.width(), size.height()), 32, QImage::Format_ARGB32_Premultiplied);
	screenAdded(screen);
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

// Dummy font database that does not scan the fonts directory to be
// used for command line tools like qmlplugindump that do not create windows
// unless DebugBackingStore is activated.
class DummyFontDatabase : public QPlatformFontDatabase
{
public:
	virtual void populateFontDatabase() {}
};

QPlatformFontDatabase *QBareIntegration::fontDatabase() const
{
	if (m_options & EnableFonts)
		return QPlatformIntegration::fontDatabase();
	if (!m_dummyFontDatabase)
		m_dummyFontDatabase = new DummyFontDatabase;
	return m_dummyFontDatabase;
}

QPlatformWindow *QBareIntegration::createPlatformWindow(QWindow *window) const
{
	QBareWindow *w = new QBareWindow(window);
	w->requestActivateWindow();
	return w;
}

QPlatformBackingStore *QBareIntegration::createPlatformBackingStore(QWindow *window) const
{
	return new QBareBackingStore(window);
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

#include "qbareintegration.h"
#include "qbarebackingstore.h"
#include "qbarescreen.h"

#include <QtGui/private/qpixmap_raster_p.h>
#include <QtGui/private/qguiapplication_p.h>
#include <qpa/qplatformwindow.h>
#include <qpa/qplatformfontdatabase.h>

#include <QtPlatformSupport/private/qgenericunixeventdispatcher_p.h>

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
	if (qEnvironmentVariableIsSet(debugBackingStoreEnvironmentVariable)
			&& qgetenv(debugBackingStoreEnvironmentVariable).toInt() > 0) {
		m_options |= DebugBackingStore | EnableFonts;
	}

	QBareScreen *mPrimaryScreen = new QBareScreen();

	mPrimaryScreen->mGeometry = QRect(0, 0, 240, 320);
	mPrimaryScreen->mDepth = 32;
	mPrimaryScreen->mFormat = QImage::Format_ARGB32_Premultiplied;

	screenAdded(mPrimaryScreen);
}

QBareIntegration::~QBareIntegration()
{
	delete m_dummyFontDatabase;
}

bool QBareIntegration::hasCapability(QPlatformIntegration::Capability cap) const
{
	switch (cap) {
	case ThreadedPixmaps: return true;
	case MultipleWindows: return true;
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
	Q_UNUSED(window);
	QPlatformWindow *w = new QPlatformWindow(window);
	w->requestActivateWindow();
	return w;
}

QPlatformBackingStore *QBareIntegration::createPlatformBackingStore(QWindow *window) const
{
	return new QBareBackingStore(window);
}

QAbstractEventDispatcher *QBareIntegration::createEventDispatcher() const
{
#ifdef Q_OS_WIN
#ifndef Q_OS_WINRT
	return new QEventDispatcherWin32;
#else // !Q_OS_WINRT
	return new QEventDispatcherWinRT;
#endif // Q_OS_WINRT
#else
	return createUnixEventDispatcher();
#endif
}

QBareIntegration *QBareIntegration::instance()
{
	return static_cast<QBareIntegration *>(QGuiApplicationPrivate::platformIntegration());
}

QT_END_NAMESPACE

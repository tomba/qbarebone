#include "qbarebackingstore.h"
#include "qbareintegration.h"
#include "qscreen.h"
#include <QtCore/qdebug.h>
#include <qpa/qplatformscreen.h>
#include <private/qguiapplication_p.h>

#include <cstdio>

QT_BEGIN_NAMESPACE

QBareBackingStore::QBareBackingStore(QWindow *window)
	: QPlatformBackingStore(window)
	, mDebug(QBareIntegration::instance()->options() & QBareIntegration::DebugBackingStore)
{
	if (mDebug)
		qDebug() << "QMinimalBackingStore::QMinimalBackingStore:" << (quintptr)this;

	printf("QBareBackingStore(wnd: %p)\n", window);
}

QBareBackingStore::~QBareBackingStore()
{
}

QPaintDevice *QBareBackingStore::paintDevice()
{
	printf("QBareBackingStore::paintDevice\n");

	return &mImage;
}

void QBareBackingStore::flush(QWindow *window, const QRegion &region, const QPoint &offset)
{
	Q_UNUSED(window);
	Q_UNUSED(region);
	Q_UNUSED(offset);

	printf("QBareBackingStore::flush\n");

	if (true) {
		static int c = 0;
		QString filename = QString("output%1.png").arg(c++, 4, 10, QLatin1Char('0'));
		qDebug() << "QMinimalBackingStore::flush() saving contents to" << filename.toLocal8Bit().constData();
		mImage.save(filename);
	}
}

void QBareBackingStore::resize(const QSize &size, const QRegion &)
{
	printf("QBareBackingStore::resize(%dx%d)\n", size.width(), size.height());

	QImage::Format format = QGuiApplication::primaryScreen()->handle()->format();
	if (mImage.size() != size)
		mImage = QImage(size, format);
}

QT_END_NAMESPACE

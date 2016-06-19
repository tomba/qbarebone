#include "qbarebackingstore.h"
#include "qbareintegration.h"
#include "qscreen.h"
#include <QtCore/qdebug.h>
#include <qpa/qplatformscreen.h>
#include <private/qguiapplication_p.h>
#include "qbareclientinterface.h"

#include <cstdio>

QT_BEGIN_NAMESPACE

QBareBackingStore::QBareBackingStore(QWindow *window, QBareIntegration* integration)
	: QPlatformBackingStore(window),
	  m_integration(integration)
{
	printf("QBareBackingStore(wnd: %p)\n", window);
}

QBareBackingStore::~QBareBackingStore()
{
}

QPaintDevice *QBareBackingStore::paintDevice()
{
	//printf("QBareBackingStore::paintDevice\n");


	QPaintDevice *p = m_integration->client()->paintDevice();
	return p;

	//return &mImage;
}

void QBareBackingStore::flush(QWindow *window, const QRegion &region, const QPoint &offset)
{
	Q_UNUSED(window);
	Q_UNUSED(region);
	Q_UNUSED(offset);

	printf("QBareBackingStore::flush(%p, %u,%u %ux%u)\n", window,
	       region.boundingRect().x(), region.boundingRect().y(),
	       region.boundingRect().width(), region.boundingRect().height());

	if (false) {
		static int c = 0;
		QString filename = QString("output%1.png").arg(c++, 4, 10, QLatin1Char('0'));
		qDebug() << "QMinimalBackingStore::flush() saving contents to" << filename.toLocal8Bit().constData();
		mImage.save(filename);
	}

	m_integration->client()->flush();
}

void QBareBackingStore::resize(const QSize &size, const QRegion &staticContents)
{
	printf("QBareBackingStore::resize(%dx%d)\n", size.width(), size.height());

	//QImage::Format format = QGuiApplication::primaryScreen()->handle()->format();
	//if (mImage.size() != size)
	//	mImage = QImage(size, format);

	//mImage = m_integration->client()->get_qimage(size);

	m_integration->client()->resize(size, staticContents);
}

QT_END_NAMESPACE

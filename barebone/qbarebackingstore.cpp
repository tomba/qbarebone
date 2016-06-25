#include "qbarebackingstore.h"
#include "qbareintegration.h"
#include "qscreen.h"
#include <QtCore/qdebug.h>
#include <qpa/qplatformscreen.h>
#include <private/qguiapplication_p.h>
#include "qbareclientinterface.h"
#include "qbarewindow.h"
#include <QPainter>

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
	printf("~QBareBackingStore\n");
}

QPaintDevice *QBareBackingStore::paintDevice()
{
	printf("QBareBackingStore::paintDevice\n");


	//QBareWindow *wnd = (QBareWindow*)window()->handle();
	//QBareScreen *scr = (QBareScreen*)wnd->screen();

	//return scr->m_backbuffer;

	return &mImage;
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

	//m_integration->client()->flush();

	QBareWindow *wnd = (QBareWindow*)this->window()->handle();
	QBareScreen *scr = (QBareScreen*)wnd->screen();

	wnd->m_store = this;

	scr->scheduleUpdate();
}

void QBareBackingStore::resize(const QSize &size, const QRegion &staticContents)
{
	printf("QBareBackingStore::resize(%dx%d)\n", size.width(), size.height());

	//QImage::Format format = QGuiApplication::primaryScreen()->handle()->format();

	QBareWindow *wnd = (QBareWindow*)window()->handle();
	QBareScreen *scr = (QBareScreen*)wnd->screen();

	if (mImage.size() != size)
		mImage = QImage(size, scr->format());
}

void QBareBackingStore::beginPaint(const QRegion&)
{
	printf("begin paint\n");
}

void QBareBackingStore::endPaint()
{
	printf("end paint\n");
}

QT_END_NAMESPACE

#include "qbarebackingstore.h"
#include "qbareintegration.h"
#include "qscreen.h"
#include <QtCore/qdebug.h>
#include <qpa/qplatformscreen.h>
#include <private/qguiapplication_p.h>
#include "qbarewindow.h"
#include <QPainter>

#include <cstdio>

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
	//printf("QBareBackingStore::paintDevice\n");

	return &m_image;
}

void QBareBackingStore::flush(QWindow *window, const QRegion &region, const QPoint &offset)
{
	//printf("QBareBackingStore::flush(%p, %u,%u %ux%u)\n", window,
	//       region.boundingRect().x(), region.boundingRect().y(),
	//       region.boundingRect().width(), region.boundingRect().height());

	QBareWindow *wnd = (QBareWindow*)this->window()->handle();
	QBareScreen *scr = (QBareScreen*)wnd->screen();

	wnd->set_store(this);

	scr->scheduleUpdate();
}

void QBareBackingStore::resize(const QSize &size, const QRegion &staticContents)
{
	printf("QBareBackingStore::resize(%dx%d)\n", size.width(), size.height());

	QBareWindow *wnd = (QBareWindow*)window()->handle();
	QBareScreen *scr = (QBareScreen*)wnd->screen();

	if (m_image.size() != size)
		m_image = QImage(size, scr->format());
}

void QBareBackingStore::beginPaint(const QRegion&)
{
	//printf("begin paint\n");
}

void QBareBackingStore::endPaint()
{
	//printf("end paint\n");
}

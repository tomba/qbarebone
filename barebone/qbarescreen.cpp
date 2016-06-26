#include "qbareintegration.h"
#include "qbarebackingstore.h"
#include "qbarescreen.h"

#include <QtGui/private/qpixmap_raster_p.h>
#include <QtGui/private/qguiapplication_p.h>
#include <qpa/qplatformwindow.h>
#include <qpa/qplatformfontdatabase.h>
#include <qpa/qplatformcursor.h>
#include "qbarewindow.h"

#include <cstdio>

QT_BEGIN_NAMESPACE

QBareScreen::QBareScreen(const QRect &geom, int depth, QImage::Format format, QBareIntegration* integration)
	: mGeometry(geom), mDepth(depth), mFormat(format), m_integration(integration)
{
	printf("QBareScreen(%dx%d)\n", geom.width(), geom.height());

	m_cursor = new QBareCursor(this);
}

QPlatformCursor* QBareScreen::cursor() const
{
	return m_cursor;
}

void QBareScreen::scheduleUpdate()
{
	if (!mUpdatePending) {
		mUpdatePending = true;
		QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
	}
}

bool QBareScreen::event(QEvent *event)
{
	if (event->type() == QEvent::UpdateRequest) {
		m_integration->client()->flush();
		mUpdatePending = false;
		return true;
	}
	return QObject::event(event);
}

void QBareScreen::draw(QImage& qimage)
{
	QPainter p;
	p.begin(&qimage);

	for (int i = 0; i < mWindowStack.length(); ++i) {
		QBareWindow* wnd = mWindowStack[i];

		QBareBackingStore* store = wnd->m_store;

		QImage& img = store->mImage;

		p.drawImage(wnd->window()->x(), wnd->window()->y(), img);
	}

	QPoint pos = m_cursor->pos();
	p.drawImage(pos, *m_cursor->m_image->image());

	p.end();
}

void QBareScreen::addWindow(QBareWindow* wnd)
{
	mWindowStack.append(wnd);
}

void QBareScreen::removeWindow(QBareWindow* wnd)
{
	mWindowStack.removeOne(wnd);
}

QT_END_NAMESPACE

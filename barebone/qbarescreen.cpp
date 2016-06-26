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

QBareScreen::QBareScreen(const QRect &geom, int depth, QImage::Format format, QBareIntegration* integration)
	: m_geometry(geom), m_depth(depth), m_format(format), m_integration(integration)
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
	if (!m_updatePending) {
		m_updatePending = true;
		QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
	}
}

bool QBareScreen::event(QEvent *event)
{
	if (event->type() == QEvent::UpdateRequest) {
		m_integration->client()->flush();
		m_updatePending = false;
		return true;
	}
	return QObject::event(event);
}

void QBareScreen::draw(QImage& qimage)
{
	QPainter p;
	p.begin(&qimage);

	for (int i = 0; i < m_windowStack.length(); ++i) {
		QBareWindow* wnd = m_windowStack[i];

		QBareBackingStore* store = wnd->store();

		const QImage& img = store->get_image();

		p.drawImage(wnd->window()->x(), wnd->window()->y(), img);
	}

	QPoint pos = m_cursor->pos();
	pos -= m_cursor->hotspot();
	p.drawPixmap(pos, m_cursor->pixmap());

	p.end();
}

void QBareScreen::addWindow(QBareWindow* wnd)
{
	m_windowStack.append(wnd);
}

void QBareScreen::removeWindow(QBareWindow* wnd)
{
	m_windowStack.removeOne(wnd);
}

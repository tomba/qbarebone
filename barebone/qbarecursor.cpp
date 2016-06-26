#include "qbarecursor.h"
#include <QtCore/QCoreApplication>
#include <QWindow>
#include "qbarescreen.h"

QBareCursor::QBareCursor(QBareScreen *screen)
	: m_screen(screen)
{
	m_cursor_image.reset(new QPlatformCursorImage(0, 0, 0, 0, 0, 0));
}

void QBareCursor::changeCursor(QCursor *cursor, QWindow *window)
{
	//printf("Change cursor\n");

	const Qt::CursorShape newShape = cursor ? cursor->shape() : Qt::ArrowCursor;

	if (newShape != Qt::BitmapCursor) {
		m_cursor_image->set(newShape);
		m_hotspot = m_cursor_image->hotspot();
		m_pixmap = QPixmap::fromImage(*m_cursor_image->image());
	} else {
		m_hotspot = cursor->hotSpot();
		m_pixmap = cursor->pixmap();
	}
}

void QBareCursor::pointerEvent(const QMouseEvent& event)
{
	//printf("PointerEvent %d, %d\n", event.globalPos().x(), event.globalPos().y());
	m_screen->scheduleUpdate();
}

void QBareCursor::setPos(const QPoint& pos)
{
	printf("setPos %u,%u\n", pos.x(), pos.y());
	QPlatformCursor::setPos(pos);
}

#pragma once

#include <qpa/qplatformcursor.h>

class QBareScreen;

class QBareCursor : public QPlatformCursor
{
public:
	QBareCursor(QBareScreen *screen);

	// QPlatformCursor
	void changeCursor(QCursor *cursor, QWindow *window);
	void pointerEvent(const QMouseEvent & event);
	void setPos(const QPoint &pos);

	const QPixmap& pixmap() const { return m_pixmap; }
	QPoint hotspot() const { return m_hotspot; }

private:
	QScopedPointer<QPlatformCursorImage> m_cursor_image;
	QBareScreen *m_screen;
	QPixmap m_pixmap;
	QPoint m_hotspot;
};

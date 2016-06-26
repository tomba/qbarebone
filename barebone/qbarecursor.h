#pragma once

#include <qpa/qplatformcursor.h>

class QBareScreen;

class QBareCursor : public QPlatformCursor
{
public:
	QBareCursor(QBareScreen *screen);
#ifndef QT_NO_CURSOR
	void changeCursor(QCursor *cursor, QWindow *window);
#endif

	virtual void pointerEvent(const QMouseEvent & event);

	virtual void setPos(const QPoint &pos);

//private:
#ifndef QT_NO_CURSOR
	QScopedPointer<QPlatformCursorImage> m_image;
#endif
	QBareScreen *m_screen;
};

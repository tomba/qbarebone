#ifndef QBARECURSOR_H
#define QBARECURSOR_H

#include <qpa/qplatformcursor.h>

QT_BEGIN_NAMESPACE

class QBareCursor : public QPlatformCursor
{
public:
	QBareCursor(QPlatformScreen *screen);
#ifndef QT_NO_CURSOR
	void changeCursor(QCursor *cursor, QWindow *window);
#endif

	virtual void pointerEvent(const QMouseEvent & event);

	virtual void setPos(const QPoint &pos);

private:
#ifndef QT_NO_CURSOR
	QScopedPointer<QPlatformCursorImage> m_image;
#endif
	QPlatformScreen *m_screen;
};

QT_END_NAMESPACE

#endif // QBARECURSOR_H

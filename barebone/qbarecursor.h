#ifndef QBARECURSOR_H
#define QBARECURSOR_H

#include <qpa/qplatformcursor.h>

class QBareScreen;

QT_BEGIN_NAMESPACE

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

QT_END_NAMESPACE

#endif // QBARECURSOR_H

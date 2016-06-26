#include "qbarecursor.h"
#include <QtCore/QCoreApplication>
#include <QWindow>
#include "qbarescreen.h"

QBareCursor::QBareCursor(QBareScreen *screen)
    : m_screen(screen)
{
    m_image.reset(new QPlatformCursorImage(0, 0, 0, 0, 0, 0));
}

void QBareCursor::changeCursor(QCursor *cursor, QWindow *)
{
    //int xSpot;
    //int ySpot;
    QPixmap map;

    //printf("Change cursor\n");

    const Qt::CursorShape newShape = cursor ? cursor->shape() : Qt::ArrowCursor;
    if (newShape != Qt::BitmapCursor) {
	m_image->set(newShape);
	//xSpot = m_image->hotspot().x();
	//ySpot = m_image->hotspot().y();
	QImage *i = m_image->image();
	map = QPixmap::fromImage(*i);
    } else {
	//QPoint point = cursor->hotSpot();
	//xSpot = point.x();
	//ySpot = point.y();
	map = cursor->pixmap();
    }

/*
    DFBResult res;
    IDirectFBDisplayLayer *layer = toDfbLayer(m_screen);
    IDirectFBSurface* surface(QDirectFbConvenience::dfbSurfaceForPlatformPixmap(map.handle()));

    res = layer->SetCooperativeLevel(layer, DLSCL_ADMINISTRATIVE);
    if (res != DFB_OK) {
	DirectFBError("Failed to set DLSCL_ADMINISTRATIVE", res);
	return;
    }

    layer->SetCursorShape(layer, surface, xSpot, ySpot);
    layer->SetCooperativeLevel(layer, DLSCL_SHARED);
    */
}

void QBareCursor::pointerEvent(const QMouseEvent& event)
{
	printf("PointerEvent %d, %d\n", event.globalPos().x(), event.globalPos().y());

	m_screen->scheduleUpdate();
}

void QBareCursor::setPos(const QPoint& pos)
{
	printf("setPos %u,%u\n", pos.x(), pos.y());
	QPlatformCursor::setPos(pos);
}

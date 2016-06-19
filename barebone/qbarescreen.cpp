#include "qbareintegration.h"
#include "qbarebackingstore.h"
#include "qbarescreen.h"

#include <QtGui/private/qpixmap_raster_p.h>
#include <QtGui/private/qguiapplication_p.h>
#include <qpa/qplatformwindow.h>
#include <qpa/qplatformfontdatabase.h>
#include <qpa/qplatformcursor.h>

#include <cstdio>

QT_BEGIN_NAMESPACE

QBareScreen::QBareScreen(const QRect &geom, int depth, QImage::Format format)
	: mGeometry(geom), mDepth(depth), mFormat(format)
{
	printf("QBareScreen(%dx%d)\n", geom.width(), geom.height());

	m_cursor = new QBareCursor(this);
}

QPlatformCursor* QBareScreen::cursor() const
{
	return m_cursor;
}

QT_END_NAMESPACE

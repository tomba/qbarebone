#pragma once

#include <qpa/qplatformscreen.h>
#include "qbarecursor.h"

QT_BEGIN_NAMESPACE

class QBareScreen : public QPlatformScreen
{
public:
	QBareScreen(const QRect& geom, int depth, QImage::Format format);

	QRect geometry() const { return mGeometry; }
	int depth() const { return mDepth; }
	QImage::Format format() const { return mFormat; }
	QString name() const { return "myscreen"; }

	QPlatformCursor *cursor() const;

private:
	QRect mGeometry;
	int mDepth;
	QImage::Format mFormat;
	QSize mPhysicalSize;
	QBareCursor* m_cursor;
};

QT_END_NAMESPACE

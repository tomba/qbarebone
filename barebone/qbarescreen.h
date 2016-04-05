#pragma once

#include <qpa/qplatformscreen.h>

QT_BEGIN_NAMESPACE

class QBareScreen : public QPlatformScreen
{
public:
	QBareScreen()
		: mDepth(32), mFormat(QImage::Format_ARGB32_Premultiplied) {}

	QRect geometry() const { return mGeometry; }
	int depth() const { return mDepth; }
	QImage::Format format() const { return mFormat; }

public:
	QRect mGeometry;
	int mDepth;
	QImage::Format mFormat;
	QSize mPhysicalSize;
};

QT_END_NAMESPACE

#pragma once

#include <QSize>
#include <QImage>
#include <QPaintDevice>

class QBareClientInterface
{
public:
	virtual ~QBareClientInterface() { }
	virtual void test() = 0;
	virtual void flush() = 0;

	virtual QPaintDevice *paintDevice() = 0;
	virtual void resize(const QSize &size, const QRegion &staticContents) = 0;
};

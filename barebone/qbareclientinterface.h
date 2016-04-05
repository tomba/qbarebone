#pragma once

#include <QSize>
#include <QImage>

class QBareClientInterface
{
public:
	virtual ~QBareClientInterface() { }
	virtual void test() = 0;
	virtual QImage get_qimage(const QSize& size) = 0;
};

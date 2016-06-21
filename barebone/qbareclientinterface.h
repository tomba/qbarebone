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
};

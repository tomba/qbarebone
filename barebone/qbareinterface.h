#pragma once

#include <QSize>

class QBareInterface
{
public:
	virtual ~QBareInterface() { }
	virtual void test() = 0;
	virtual void add_screen(QSize size) = 0;
};

#pragma once

#include <QSize>

class QBareClientInterface;

class QBareInterface
{
public:
	virtual ~QBareInterface() { }
	virtual void test() = 0;
	virtual void add_screen(QSize size) = 0;
	virtual void install_client(QBareClientInterface* client) = 0;
};

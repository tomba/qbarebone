#pragma once

#include <QSize>

class QBareScreenInterface;

class QBareClientInterface
{
public:
	virtual ~QBareClientInterface() { }
	virtual void flush(QBareScreenInterface* screen) = 0;
};

class QBareScreenInterface
{
public:
	virtual void draw(QImage& qimage) = 0;
};

class QBareInterface
{
public:
	virtual ~QBareInterface() { }
	virtual QBareScreenInterface* add_screen(QSize size, QString name) = 0;
	virtual void install_client(QBareClientInterface* client) = 0;
};

#pragma once

#include <QSize>

class QBareIntegration;

class QBareInterface
{
public:
	QBareInterface(QBareIntegration *integration);

	virtual void test();

	virtual void add_screen(QSize size);

private:
	QBareIntegration *m_integration;
};

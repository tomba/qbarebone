#include "qbareinterface.h"
#include <cstdio>
#include "qbareintegration.h"
#include "qbarescreen.h"

QBareInterface::QBareInterface(QBareIntegration *integration)
	: m_integration(integration)
{

}

void QBareInterface::test()
{
	printf("TESTI\n");
}

void QBareInterface::add_screen(QSize size)
{
	m_integration->add_screen(size);
}

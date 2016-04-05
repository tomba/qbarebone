#include "qbarenativeinterface.h"
#include "qbareinterface.h"
#include "qbareintegration.h"

#include <cstdio>

QBareNativeInterface::QBareNativeInterface(QBareIntegration *integration)
	: m_integration(integration)
{
}

void *QBareNativeInterface::nativeResourceForIntegration(const QByteArray &resource)
{
	if (resource == "main")
		return dynamic_cast<QBareInterface*>(m_integration);

	return 0;
}

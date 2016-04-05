#include "qbarenativeinterface.h"
#include "qbareinterface.h"

QBareNativeInterface::QBareNativeInterface(QBareIntegration *integration)
	: m_integration(integration)
{
	m_mainIface = new QBareInterface(m_integration);
}

void *QBareNativeInterface::nativeResourceForIntegration(const QByteArray &resource)
{
	if (resource == "main")
		return m_mainIface;

	return 0;
}

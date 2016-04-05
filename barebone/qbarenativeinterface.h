#pragma once

#include <qpa/qplatformnativeinterface.h>

class QBareIntegration;

class QBareNativeInterface : public QPlatformNativeInterface
{
public:
	QBareNativeInterface(QBareIntegration* integration);

	void* nativeResourceForIntegration(const QByteArray &resource);

private:
	QBareIntegration* m_integration;
};

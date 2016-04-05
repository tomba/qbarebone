#pragma once

#include <qpa/qplatformintegration.h>
#include "qbarenativeinterface.h"
#include "qbareinterface.h"

QT_BEGIN_NAMESPACE

class QBareIntegration : public QPlatformIntegration, public QBareInterface
{
public:
	explicit QBareIntegration(const QStringList &parameters);
	~QBareIntegration();

	bool hasCapability(QPlatformIntegration::Capability cap) const;
	QPlatformFontDatabase *fontDatabase() const;

	QPlatformWindow *createPlatformWindow(QWindow *window) const;
	QPlatformBackingStore *createPlatformBackingStore(QWindow *window) const;
	QAbstractEventDispatcher *createEventDispatcher() const;

	static QBareIntegration *instance();

	QPlatformNativeInterface *nativeInterface() const;
	QPlatformServices *services() const;

	virtual void add_screen(QSize size);
	virtual void test();
	virtual void install_client(QBareClientInterface *client);

	QBareClientInterface* client() const { return m_clientInterface; }

private:
	QBareNativeInterface* m_nativeInterface;
	QBareClientInterface* m_clientInterface;
};

QT_END_NAMESPACE

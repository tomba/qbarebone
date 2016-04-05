#pragma once

#include <qpa/qplatformintegration.h>
#include "qbarenativeinterface.h"

QT_BEGIN_NAMESPACE

class QBareIntegration : public QPlatformIntegration
{
public:
	enum Options { // Options to be passed on command line or determined from environment
		DebugBackingStore = 0x1,
		EnableFonts = 0x2
	};

	explicit QBareIntegration(const QStringList &parameters);
	~QBareIntegration();

	bool hasCapability(QPlatformIntegration::Capability cap) const;
	QPlatformFontDatabase *fontDatabase() const;

	QPlatformWindow *createPlatformWindow(QWindow *window) const;
	QPlatformBackingStore *createPlatformBackingStore(QWindow *window) const;
	QAbstractEventDispatcher *createEventDispatcher() const;

	unsigned options() const { return m_options; }

	static QBareIntegration *instance();

	QPlatformNativeInterface *nativeInterface() const;
	QPlatformServices *services() const;

	void add_screen(QSize size);

private:
	mutable QPlatformFontDatabase *m_dummyFontDatabase;
	unsigned m_options;
	QBareNativeInterface* m_nativeInterface;
};

QT_END_NAMESPACE

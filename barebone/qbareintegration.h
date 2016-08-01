#pragma once

#include <qpa/qplatformintegration.h>
#include "qbarenativeinterface.h"
#include "qbareinterface.h"
#include "qbarescreen.h"

class QEvdevKeyboardManager;
class QEvdevMouseManager;

class QBareIntegration : public QPlatformIntegration, public QBareInterface
{
public:
	explicit QBareIntegration(const QStringList &parameters);
	~QBareIntegration();

	void initialize();

	bool hasCapability(QPlatformIntegration::Capability cap) const;
	QPlatformFontDatabase *fontDatabase() const;

	QPlatformWindow *createPlatformWindow(QWindow *window) const;
	QPlatformBackingStore *createPlatformBackingStore(QWindow *window) const;
	QAbstractEventDispatcher *createEventDispatcher() const;

	static QBareIntegration *instance();

	QPlatformNativeInterface *nativeInterface() const;
	QPlatformServices *services() const;

	// QBareInterface
	QBareScreenInterface* add_screen(QSize size);
	void install_client(QBareClientInterface *client);

	QBareClientInterface* client() const { return m_clientInterface; }

private:
	void createInputHandlers();

	QPlatformInputContext* m_inputContext = nullptr;
	QBareNativeInterface* m_nativeInterface = nullptr;
	QBareClientInterface* m_clientInterface = nullptr;
	QEvdevKeyboardManager* m_kbdMgr = nullptr;
	QEvdevMouseManager* m_mouseMgr = nullptr;
	QScopedPointer<QPlatformFontDatabase> m_fontDatabase;
};

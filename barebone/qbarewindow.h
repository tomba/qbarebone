#pragma once

#include <qpa/qplatformwindow.h>

class QBareBackingStore;

class QBareWindow : public QPlatformWindow
{
public:
	QBareWindow(QWindow *window);
	virtual ~QBareWindow();

	// QPlatformWindow
	void setVisible(bool visible);
	void invalidateSurface();
	void requestUpdate();
	bool setKeyboardGrabEnabled(bool) { return false; }
	bool setMouseGrabEnabled(bool) { return false; }
	void raise();
	void lower();

	void set_store(QBareBackingStore* store) { m_store = store; }
	QBareBackingStore* store() const { return m_store; }

private:
	QBareBackingStore* m_store;
};

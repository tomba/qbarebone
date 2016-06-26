#pragma once

#include <qpa/qplatformwindow.h>

class QBareBackingStore;

class QBareWindow : public QPlatformWindow
{
public:
	QBareWindow(QWindow *window);
	virtual ~QBareWindow();

	// QPlatformWindow
	virtual void setVisible(bool visible);
	virtual void invalidateSurface();
	virtual void requestUpdate();

	void set_backing_store(QBareBackingStore* store) { m_store = store; }
	QBareBackingStore* store() const { return m_store; }

private:
	QBareBackingStore* m_store;
};

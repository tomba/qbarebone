#pragma once

#include <qpa/qplatformwindow.h>

class QBareBackingStore;

class QBareWindow : public QPlatformWindow
{
public:
	QBareWindow(QWindow *window);
	virtual ~QBareWindow();

	virtual void setVisible(bool visible);

	virtual void invalidateSurface();
	virtual void requestUpdate();

	QBareBackingStore* m_store;
};

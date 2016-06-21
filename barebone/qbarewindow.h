#pragma once

#include <qpa/qplatformwindow.h>

class QBareBackingStore;

class QBareWindow : public QPlatformWindow
{
public:
	QBareWindow(QWindow *window);
	virtual ~QBareWindow();

	virtual void setVisible(bool visible);

	QBareBackingStore* m_store;
};

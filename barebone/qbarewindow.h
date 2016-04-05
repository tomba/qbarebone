#pragma once

#include <qpa/qplatformwindow.h>

class QBareWindow : public QPlatformWindow
{
public:
	QBareWindow(QWindow *window);
};

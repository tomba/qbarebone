#include "qbarewindow.h"

#include <cstdio>

QT_BEGIN_NAMESPACE

QBareWindow::QBareWindow(QWindow *window)
	:QPlatformWindow(window)
{
	printf("QBareWindow(%p, %dx%d)\n", window, window->width(), window->height());

}

QT_END_NAMESPACE

#include "qbarewindow.h"

#include <cstdio>

QT_BEGIN_NAMESPACE

QBareWindow::QBareWindow(QWindow *window)
	:QPlatformWindow(window)
{
	printf("QBareWindow(%p, %d,%d %dx%d)\n", window,
	       window->x(), window->y(), window->width(), window->height());

}

QT_END_NAMESPACE

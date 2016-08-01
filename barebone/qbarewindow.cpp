#include "qbarewindow.h"
#include "qbarescreen.h"

#include <cstdio>

QBareWindow::QBareWindow(QWindow *window)
	:QPlatformWindow(window)
{
	static QAtomicInt winIdGenerator(1);
	m_windowId = winIdGenerator.fetchAndAddRelaxed(1);

	printf("QBareWindow(%p, %d,%d %dx%d)\n", window,
	       window->x(), window->y(), window->width(), window->height());
}

QBareWindow::~QBareWindow()
{
	printf("~QBareWindow\n");
}

void QBareWindow::setVisible(bool visible)
{
	QPlatformWindow::setVisible(visible);

	QBareScreen* scr = (QBareScreen*)screen();

	if (visible)
		scr->addWindow(this);
	else
		scr->removeWindow(this);
}

void QBareWindow::invalidateSurface()
{
	printf("wnd: invalidate\n");
}

void QBareWindow::requestUpdate()
{
	printf("wnd: requestUpdate\n");
}

void QBareWindow::raise()
{
	QBareScreen* scr = (QBareScreen*)screen();
	scr->raise(this);
}

void QBareWindow::lower()
{
	QBareScreen* scr = (QBareScreen*)screen();
	scr->lower(this);
}

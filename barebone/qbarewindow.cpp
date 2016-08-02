#include "qbarewindow.h"
#include "qbarescreen.h"

#include <cstdio>

QBareWindow::QBareWindow(QWindow *window)
	:QPlatformWindow(window)
{
	static QAtomicInt winIdGenerator(1);
	m_windowId = winIdGenerator.fetchAndAddRelaxed(1);

	printf("QBareWindow(%u, %d,%d %dx%d)\n", winId(),
	       window->x(), window->y(), window->width(), window->height());
}

QBareWindow::~QBareWindow()
{
	printf("~QBareWindow(%u)\n", winId());
}

void QBareWindow::setVisible(bool visible)
{
	printf("QBareWindow::setVisible(%u, scr %s, vis %d)\n", winId(), screen()->name().toLatin1().data(), visible);

	QPlatformWindow::setVisible(visible);

	QBareScreen* scr = (QBareScreen*)screen();

	if (visible)
		scr->addWindow(this);
	else
		scr->removeWindow(this);
}

void QBareWindow::invalidateSurface()
{
	printf("QBareWindow::invalidateSurface(%u)\n", winId());
}

void QBareWindow::requestUpdate()
{
	printf("QBareWindow::requestUpdate(%u)\n", winId());
}

void QBareWindow::raise()
{
	printf("QBareWindow::raise(%u)\n", winId());
	QBareScreen* scr = (QBareScreen*)screen();
	scr->raise(this);
}

void QBareWindow::lower()
{
	printf("QBareWindow::lower(%u)\n", winId());
	QBareScreen* scr = (QBareScreen*)screen();
	scr->lower(this);
}

#include "mainwindow.h"
#include <QApplication>
#include <QScreen>
#include <QtDebug>

#include "qbareclient.h"
#include "trianglewindow.h"
#include "rasterwindow.h"

#include <cstdio>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/kd.h>

int main(int argc, char *argv[])
{
	//int tty = open("/dev/tty1", O_RDWR);
	//ioctl(tty, KDSETMODE, KD_GRAPHICS);


	printf("Create App\n");
	QApplication a(argc, argv);

	QBareClient* bb = nullptr;

	if (a.platformName() == "barebone")
		bb = new QBareClient(a);

#if 1
	auto screens = a.screens();

	for (int i = 0; i < screens.size(); ++i) {
		//qDebug() << QString("Create window on screen %1").arg(screens.at(i)->name());

		MainWindow *w = new MainWindow();
		w->move(50, 0);
		w->show();

		//MainWindow *w2 = new MainWindow();
		//w2->move(300, 50);
		//w2->show();
	}
#endif
#if 0
	{
		QSurfaceFormat format;
		format.setSamples(16);

		TriangleWindow *window = new TriangleWindow();
		window->setFormat(format);
		window->resize(640, 480);
		window->show();

		window->setAnimating(true);
	}
#endif
#if 0
	{
		RasterWindow *w = new RasterWindow();
		w->show();
	}
#endif
	printf("Enter mainloop\n");
	int r = a.exec();

	if (bb)
		delete bb;

	return r;
}

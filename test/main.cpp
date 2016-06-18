#include "mainwindow.h"
#include <QApplication>
#include <QScreen>
#include <QtDebug>

#include "qbareclient.h"
#include "trianglewindow.h"
#include "rasterwindow.h"

#include <cstdio>

int main(int argc, char *argv[])
{
	printf("Create App\n");
	QApplication app(argc, argv);

	QBareClient* bb = nullptr;

	if (app.platformName() == "barebone") {
		printf("Create BB\n");
		bb = new QBareClient(app);
	}

#if 0
	{
		printf("Open MainWindow\n");

		auto screens = app.screens();

		for (int i = 0; i < screens.size(); ++i) {
			qDebug() << QString("Create window on screen %1").arg(screens.at(i)->name());

			MainWindow *w = new MainWindow();
			w->show();
		}
	}
#endif
#if 0
	{
		printf("Open TriangleWindow\n");
		QSurfaceFormat format;
		format.setSamples(16);

		TriangleWindow *window = new TriangleWindow();
		window->setFormat(format);
		window->resize(640, 480);
		window->show();

		window->setAnimating(true);
	}
#endif
#if 1
	{
		printf("Open RasterWindow\n");
		RasterWindow *w = new RasterWindow();
		w->show();
	}
#endif
	printf("Enter mainloop\n");
	int r = app.exec();

	if (bb)
		delete bb;

	return r;
}

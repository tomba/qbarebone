#include "mainwindow.h"
#include <QApplication>
#include <QScreen>
#include <QtDebug>
#include <qnamespace.h>

#include "qbareclient.h"
#include "trianglewindow.h"
#include "rasterwindow.h"

int run_moveblocks(QApplication& app);

int main(int argc, char *argv[])
{
	printf("Create App\n");

	// XXX with high dpi scaling, plugin crashes without initial screen
	QCoreApplication::setAttribute(Qt::AA_DisableHighDpiScaling, true);

	QApplication app(argc, argv);

	QBareClient* bb = nullptr;

	if (app.platformName() == "barebone")
		bb = new QBareClient(app);

#if 1
	auto screens = app.screens();

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
	app.exec();

	//run_moveblocks(app);

	if (bb)
		delete bb;

	return 0;
}

#include "mainwindow.h"
#include <QApplication>
#include <QScreen>
#include <QtDebug>
#include <qnamespace.h>
#include <QDesktopWidget>

#include "qbareclient.h"
#include "trianglewindow.h"
#include "rasterwindow.h"
#include "kmsmanager.h"
#include <kms++/kms++.h>

QWidget* create_moveblocks();
void dump_screens(QGuiApplication& a);

using namespace kms;

int main(int argc, char *argv[])
{
	printf("Create App\n");

#ifdef AA_DisableHighDpiScaling
	// XXX with high dpi scaling, plugin crashes without initial screen
	QCoreApplication::setAttribute(AA_DisableHighDpiScaling, true);
#endif

	QApplication app(argc, argv);

	if (app.platformName() != "barebone") {
		printf("This app requires QBareBone platform\n");
		return -1;
	}

	Card card;

	KmsManager* mgr = new KmsManager(card)

	QBareClient* bb = new QBareClient(app, mgr);

	//dump_screens(app);

	QList<QScreen*> screens = app.screens();
	QScreen* lcd = screens[0];
	QScreen* hdmi = screens[1];

	int test_num = 0;

	if (test_num == 0) {
		MainWindow *w1 = new MainWindow();
		//w1->winId();
		//w1->windowHandle()->setScreen(lcd);
		w1->move(50, 50);
		w1->show();

		//MainWindow *w2 = new MainWindow();
		//w2->winId();
		//w2->windowHandle()->setScreen(hdmi);
		//w2->move(300, 50);
		//w2->show();

	} else if (test_num == 1) {
		QSurfaceFormat format;
		format.setSamples(16);

		TriangleWindow *window = new TriangleWindow();
		window->setFormat(format);
		window->resize(640, 480);
		window->show();

		window->setAnimating(true);
	} else if (test_num == 2) {
		RasterWindow *w = new RasterWindow();
		w->show();
	} else if (test_num == 3) {
		QWidget* moveblk = create_moveblocks();
		moveblk->windowHandle()->setScreen(hdmi);
		moveblk->hide();
		moveblk->show();
	}

	printf("Enter mainloop\n");
	app.exec();

	delete bb;

	return 0;
}

QString Orientation(Qt::ScreenOrientation orientation)
{
	switch (orientation) {
	case Qt::PrimaryOrientation           : return "Primary";
	case Qt::LandscapeOrientation         : return "Landscape";
	case Qt::PortraitOrientation          : return "Portrait";
	case Qt::InvertedLandscapeOrientation : return "Inverted landscape";
	case Qt::InvertedPortraitOrientation  : return "Inverted portrait";
	default                               : return "Unknown";
	}
}

void dump_screens(QGuiApplication& a)
{
	qDebug() << "Number of screens:" << QGuiApplication::screens().size();

	qDebug() << "Primary screen:" << QGuiApplication::primaryScreen()->name();

	foreach (QScreen *screen, QGuiApplication::screens()) {
		qDebug() << "Information for screen:" << screen->name();
		qDebug() << "  Available geometry:" << screen->availableGeometry().x() << screen->availableGeometry().y() << screen->availableGeometry().width() << "x" << screen->availableGeometry().height();
		qDebug() << "  Available size:" << screen->availableSize().width() << "x" << screen->availableSize().height();
		qDebug() << "  Available virtual geometry:" << screen->availableVirtualGeometry().x() << screen->availableVirtualGeometry().y() << screen->availableVirtualGeometry().width() << "x" << screen->availableVirtualGeometry().height();
		qDebug() << "  Available virtual size:" << screen->availableVirtualSize().width() << "x" << screen->availableVirtualSize().height();
		qDebug() << "  Depth:" << screen->depth() << "bits";
		qDebug() << "  Geometry:" << screen->geometry().x() << screen->geometry().y() << screen->geometry().width() << "x" << screen->geometry().height();
		qDebug() << "  Logical DPI:" << screen->logicalDotsPerInch();
		qDebug() << "  Logical DPI X:" << screen->logicalDotsPerInchX();
		qDebug() << "  Logical DPI Y:" << screen->logicalDotsPerInchY();
		qDebug() << "  Orientation:" << Orientation(screen->orientation());
		qDebug() << "  Physical DPI:" << screen->physicalDotsPerInch();
		qDebug() << "  Physical DPI X:" << screen->physicalDotsPerInchX();
		qDebug() << "  Physical DPI Y:" << screen->physicalDotsPerInchY();
		qDebug() << "  Physical size:" << screen->physicalSize().width() << "x" << screen->physicalSize().height() << "mm";
		qDebug() << "  Primary orientation:" << Orientation(screen->primaryOrientation());
		qDebug() << "  Refresh rate:" << screen->refreshRate() << "Hz";
		qDebug() << "  Size:" << screen->size().width() << "x" << screen->size().height();
		qDebug() << "  Virtual geometry:" << screen->virtualGeometry().x() << screen->virtualGeometry().y() << screen->virtualGeometry().width() << "x" << screen->virtualGeometry().height();
		qDebug() << "  Virtual size:" << screen->virtualSize().width() << "x" << screen->virtualSize().height();
	}
}

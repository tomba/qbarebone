#include "mainwindow.h"
#include <QApplication>
#include <QPluginLoader>

#include <qpa/qplatformnativeinterface.h>

#include "qbareinterface.h"

#include <cstdio>

int main(int argc, char *argv[])
{
	QApplication::addLibraryPath(".");
/*
	QApplication::addLibraryPath("./platforms");
	QPluginLoader loader("barebone");
	if (!loader.load()) {
		printf("load failed\n");
		return 0;
	}
*/

	printf("Create App\n");
	QApplication a(argc, argv);

	printf("PLATNAME %s\n", a.platformName().toLatin1().data());

	if (a.platformName() == "barebone") {
		QPlatformNativeInterface *native = a.platformNativeInterface();
		printf("NATIVE %p\n", native);

		QBareInterface* bare = (QBareInterface*)native->nativeResourceForIntegration("main");

		bare->test();
		bare->add_screen(QSize(1000, 1000));
	}

	printf("Create window\n");
	MainWindow w;
	w.show();

	printf("Enter mainloop\n");
	return a.exec();
}

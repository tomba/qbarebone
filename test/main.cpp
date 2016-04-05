#include "mainwindow.h"
#include <QApplication>

#include <cstdio>

int main(int argc, char *argv[])
{
	QApplication::addLibraryPath(".");

	printf("Create App\n");
	QApplication a(argc, argv);
	MainWindow w;
	w.show();

	printf("Enter mainloop\n");
	return a.exec();
}

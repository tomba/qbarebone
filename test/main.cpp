#include "mainwindow.h"
#include <QApplication>

#include "qbareclient.h"

#include <cstdio>

int main(int argc, char *argv[])
{
	printf("Create App\n");
	QApplication a(argc, argv);

	QBareClient* bb = nullptr;

	if (a.platformName() == "barebone") {
		printf("Create BB\n");
		bb = new QBareClient(a);
	}

	printf("Create window\n");
	MainWindow w;
	w.show();

	printf("Enter mainloop\n");
	int r = a.exec();

	if (bb)
		delete bb;

	return r;
}

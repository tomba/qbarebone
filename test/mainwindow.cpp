#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPropertyAnimation>
#include <QMouseEvent>
#include <QStringListModel>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	QStringList list;
	for (int i = 0; i < 100; ++i)
		list.append(QString::asprintf("item%d", i));

	QStringListModel* model = new QStringListModel(list, this);
	ui->listView->setModel(model);

	QLabel *label = new QLabel("Message");
	statusBar()->addWidget(label);


	connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::on_quit);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_quit()
{
	QApplication::quit();

}


void MainWindow::on_pushButton_clicked()
{
	ui->lcdNumber->display(ui->lcdNumber->intValue() + 1);
}

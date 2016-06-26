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
	list.append("item1");
	list.append("item2");
	list.append("item3");
	list.append("item4");
	list.append("item5");
	list.append("item6");
	list.append("item7");

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

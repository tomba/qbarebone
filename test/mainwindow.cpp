#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPropertyAnimation>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);


	QPropertyAnimation* animation = new QPropertyAnimation(ui->pushButton, "geometry");
	animation->setDuration(2000);
	animation->setStartValue(QRect(0, 0, 100, 30));
	animation->setEndValue(QRect(50, 50, 100, 30));
	animation->setLoopCount(10000);

	animation->start();
}

MainWindow::~MainWindow()
{
	delete ui;
}


void MainWindow::on_pushButton_clicked()
{
	ui->lcdNumber->display(ui->lcdNumber->intValue() + 1);
}

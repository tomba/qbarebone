#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPropertyAnimation>
#include <QMouseEvent>

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

	setMouseTracking(true);
	qApp->installEventFilter(this);
}

MainWindow::~MainWindow()
{
	setMouseTracking(false);
	qApp->removeEventFilter(this);

	delete ui;
}


void MainWindow::on_pushButton_clicked()
{
	ui->lcdNumber->display(ui->lcdNumber->intValue() + 1);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
	if (event->type() == QEvent::MouseMove)
	{
		QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
		printf("Mouse move %d,%d\n", mouseEvent->pos().x(), mouseEvent->pos().y());
	}

	return false;
}

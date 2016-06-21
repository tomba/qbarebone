#pragma once

#include <qpa/qplatformscreen.h>
#include "qbarecursor.h"

QT_BEGIN_NAMESPACE

class QBareIntegration;
class QBareWindow;

class QBareScreenInterface
{
public:
	virtual void setBackbuffer(QImage* qimage) = 0;
};

class QBareScreen : public QPlatformScreen, public QBareScreenInterface
{
public:
	QBareScreen(const QRect& geom, int depth, QImage::Format format, QBareIntegration* integration);

	QRect geometry() const { return mGeometry; }
	int depth() const { return mDepth; }
	QImage::Format format() const { return mFormat; }
	QString name() const { return "myscreen"; }

	QPlatformCursor *cursor() const;

	void setBackbuffer(QImage* qimage);

	void present();

	void addWindow(QBareWindow* wnd);
	void removeWindow(QBareWindow* wnd);

//private:
	QRect mGeometry;
	int mDepth;
	QImage::Format mFormat;
	QSize mPhysicalSize;
	QBareCursor* m_cursor;
	QImage* m_backbuffer;
	QBareIntegration* m_integration;

	QList<QBareWindow*> mWindowStack;
};

QT_END_NAMESPACE

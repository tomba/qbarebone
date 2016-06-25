#pragma once

#include <qpa/qplatformscreen.h>
#include "qbarecursor.h"

QT_BEGIN_NAMESPACE

class QBareIntegration;
class QBareWindow;

class QBareScreenInterface
{
public:
	virtual void draw(QImage& qimage) = 0;
};

class QBareScreen : public QObject, public QPlatformScreen, public QBareScreenInterface
{
	Q_OBJECT
public:
	QBareScreen(const QRect& geom, int depth, QImage::Format format, QBareIntegration* integration);

	QRect geometry() const { return mGeometry; }
	int depth() const { return mDepth; }
	QImage::Format format() const { return mFormat; }
	QString name() const { return "myscreen"; }

	QPlatformCursor *cursor() const;

	void present();

	void addWindow(QBareWindow* wnd);
	void removeWindow(QBareWindow* wnd);

	virtual void draw(QImage& qimage);

protected:
	bool event(QEvent *event) Q_DECL_OVERRIDE;

public:
//private:
	QRect mGeometry;
	int mDepth;
	QImage::Format mFormat;
	QSize mPhysicalSize;
	QBareCursor* m_cursor;
	QBareIntegration* m_integration;

	QList<QBareWindow*> mWindowStack;

	bool mUpdatePending;
	void scheduleUpdate();
};

QT_END_NAMESPACE

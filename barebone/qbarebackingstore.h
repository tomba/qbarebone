#pragma once

#include <qpa/qplatformbackingstore.h>
#include <qpa/qplatformwindow.h>
#include <QtGui/QImage>

QT_BEGIN_NAMESPACE

class QBareIntegration;

class QBareBackingStore : public QPlatformBackingStore
{
public:
	QBareBackingStore(QWindow *window, QBareIntegration* integration);
	virtual ~QBareBackingStore();

	// QPlatformBackingStore
	QPaintDevice *paintDevice();
	void flush(QWindow *window, const QRegion &region, const QPoint &offset);
	void resize(const QSize &size, const QRegion &staticContents);
	virtual void beginPaint(const QRegion &);
	virtual void endPaint();

//private:
	QBareIntegration* m_integration;
	QImage mImage;
};

QT_END_NAMESPACE

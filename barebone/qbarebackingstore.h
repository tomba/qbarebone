#pragma once

#include <qpa/qplatformbackingstore.h>
#include <qpa/qplatformwindow.h>
#include <QtGui/QImage>

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
	void beginPaint(const QRegion &);
	void endPaint();

	const QImage& get_image() const { return m_image; }

private:
	QBareIntegration* m_integration;
	QImage m_image;
};

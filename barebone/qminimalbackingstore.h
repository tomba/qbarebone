#ifndef QBACKINGSTORE_MINIMAL_H
#define QBACKINGSTORE_MINIMAL_H

#include <qpa/qplatformbackingstore.h>
#include <qpa/qplatformwindow.h>
#include <QtGui/QImage>

QT_BEGIN_NAMESPACE

class QMinimalBackingStore : public QPlatformBackingStore
{
public:
	QMinimalBackingStore(QWindow *window);
	~QMinimalBackingStore();

	QPaintDevice *paintDevice();
	void flush(QWindow *window, const QRegion &region, const QPoint &offset);
	void resize(const QSize &size, const QRegion &staticContents);

private:
	QImage mImage;
	const bool mDebug;
};

QT_END_NAMESPACE

#endif

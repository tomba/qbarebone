#ifndef QPLATFORMINTEGRATION_MINIMAL_H
#define QPLATFORMINTEGRATION_MINIMAL_H

#include <qpa/qplatformintegration.h>
#include <qpa/qplatformscreen.h>

QT_BEGIN_NAMESPACE

class QMinimalScreen : public QPlatformScreen
{
public:
	QMinimalScreen()
		: mDepth(32), mFormat(QImage::Format_ARGB32_Premultiplied) {}

	QRect geometry() const { return mGeometry; }
	int depth() const { return mDepth; }
	QImage::Format format() const { return mFormat; }

public:
	QRect mGeometry;
	int mDepth;
	QImage::Format mFormat;
	QSize mPhysicalSize;
};

class QMinimalIntegration : public QPlatformIntegration
{
public:
	enum Options { // Options to be passed on command line or determined from environment
		DebugBackingStore = 0x1,
		EnableFonts = 0x2
	};

	explicit QMinimalIntegration(const QStringList &parameters);
	~QMinimalIntegration();

	bool hasCapability(QPlatformIntegration::Capability cap) const;
	QPlatformFontDatabase *fontDatabase() const;

	QPlatformWindow *createPlatformWindow(QWindow *window) const;
	QPlatformBackingStore *createPlatformBackingStore(QWindow *window) const;
	QAbstractEventDispatcher *createEventDispatcher() const;

	unsigned options() const { return m_options; }

	static QMinimalIntegration *instance();

private:
	mutable QPlatformFontDatabase *m_dummyFontDatabase;
	unsigned m_options;
};

QT_END_NAMESPACE

#endif

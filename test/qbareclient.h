#ifndef QBARECLIENT_H
#define QBARECLIENT_H

#include <QObject>
#include <QImage>
#include "qbareclientinterface.h"

#include <kms++/kms++.h>

struct QEvdevKeyboardManager;
struct QEvdevMouseManager;

class QFbVtHandler;

class QBareClient : public QObject, public QBareClientInterface
{
	Q_OBJECT
public:
	QBareClient(QApplication& a);

	// QBareClient
	virtual void test();
	virtual void flush();

	virtual QPaintDevice *paintDevice();
	virtual void resize(const QSize &size, const QRegion &staticContents);

signals:

public slots:

private:
	kms::Card* m_card;
	kms::Connector* m_conn;
	kms::Crtc* m_crtc;

	QEvdevKeyboardManager* m_keyManager;
	QEvdevMouseManager* m_mouseManager;

	kms::DumbFramebuffer* m_fb;

	QImage m_image;

	unsigned m_cur_fb = 0;
};

#endif // QBARECLIENT_H

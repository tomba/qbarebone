#ifndef QBARECLIENT_H
#define QBARECLIENT_H

#include <QObject>
#include <QImage>
#include "qbareclientinterface.h"
#include "qbarescreen.h"

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

signals:

public slots:

private:
	kms::Card* m_card;
	kms::Connector* m_conn;
	kms::Crtc* m_crtc;

	QEvdevKeyboardManager* m_keyManager;
	QEvdevMouseManager* m_mouseManager;

	QBareScreenInterface* m_screen;

	kms::DumbFramebuffer* m_fb;

	QImage m_image;

	unsigned m_cur_fb = 0;
};

#endif // QBARECLIENT_H

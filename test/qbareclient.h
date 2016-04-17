#ifndef QBARECLIENT_H
#define QBARECLIENT_H

#include <QObject>
#include "qbareclientinterface.h"

#include "kms++.h"

struct QEvdevKeyboardManager;
struct QEvdevMouseManager;

class QBareClient : public QObject, public QBareClientInterface
{
	Q_OBJECT
public:
	QBareClient(QApplication& a);
	void test();
	virtual QImage get_qimage(const QSize& size);

signals:

public slots:

private:
	kms::Card* m_card;
	kms::DumbFramebuffer* m_fb;
	QEvdevKeyboardManager* m_keyManager;
	QEvdevMouseManager* m_mouseManager;
};

#endif // QBARECLIENT_H

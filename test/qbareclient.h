#ifndef QBARECLIENT_H
#define QBARECLIENT_H

#include <QObject>
#include "qbareclientinterface.h"

#include <kms++/kms++.h>

struct QEvdevKeyboardManager;
struct QEvdevMouseManager;

class GbmDevice;
class EglState;
class GbmEglSurface;

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
	struct gbm_bo* m_bo;
	QEvdevKeyboardManager* m_keyManager;
	QEvdevMouseManager* m_mouseManager;

	GbmDevice* m_gbm;
	EglState* m_eglState;
	GbmEglSurface* m_surface;
};

#endif // QBARECLIENT_H

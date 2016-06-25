#ifndef QBARECLIENT_H
#define QBARECLIENT_H

#include <QObject>
#include <QImage>
#include <QSocketNotifier>
#include "qbareclientinterface.h"
#include "qbarescreen.h"

#include <kms++/kms++.h>
#include <vector>

struct QEvdevKeyboardManager;
struct QEvdevMouseManager;

class QBareClient : public QObject, public QBareClientInterface, public kms::PageFlipHandlerBase
{
	Q_OBJECT
public:
	QBareClient(QApplication& a);

	// QBareClient
	virtual void test();
	virtual void flush();

	virtual void handle_page_flip(uint32_t frame, double time);

signals:

public slots:
	void drmEvent();

private:
	kms::Card* m_card;
	kms::Connector* m_conn;
	kms::Crtc* m_crtc;

	QEvdevKeyboardManager* m_keyManager;
	QEvdevMouseManager* m_mouseManager;

	QBareScreenInterface* m_screen;

	std::vector<kms::DumbFramebuffer*> m_free_fbs;
	std::vector<kms::DumbFramebuffer*> m_ready_fbs;
	kms::DumbFramebuffer* m_queued_fb = 0;
	kms::DumbFramebuffer* m_display_fb = 0;

	QSocketNotifier* m_sockNotifier;

	bool m_pending_draw = false;
};

#endif // QBARECLIENT_H

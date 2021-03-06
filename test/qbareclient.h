#pragma once

#include <QObject>
#include <QSocketNotifier>
#include <QApplication>
#include <QQueue>
#include <QList>

#include <kms++/kms++.h>

#include "qbareinterface.h"
#include "kmsmanager.h"

struct QKmsDisplay : public kms::PageFlipHandlerBase
{
	kms::Connector* m_conn;
	kms::Crtc* m_crtc;
	kms::Plane* m_primary;

	QBareScreenInterface* m_screen;

	QQueue<kms::DumbFramebuffer*> m_free_fbs;
	QQueue<kms::DumbFramebuffer*> m_ready_fbs;
	kms::DumbFramebuffer* m_queued_fb = 0;
	kms::DumbFramebuffer* m_display_fb = 0;

	bool m_pending_draw = false;

	void flush();

	// PageFlipHandlerBase
	virtual void handle_page_flip(uint32_t frame, double time);
};


class QBareClient : public QObject, public QBareClientInterface
{
	Q_OBJECT
public:
	QBareClient(QApplication& a, KmsManager* mgr);

private:
	// QBareClientInterface
	virtual void flush(QBareScreenInterface* screen);

signals:

public slots:
	void drmEvent();

private:
	kms::Card* m_card;

	QSocketNotifier* m_sockNotifier;

	QKmsDisplay m_lcd;
	QKmsDisplay m_hdmi;

};

#pragma once

#include <QObject>
#include <QSocketNotifier>

#include <kms++/kms++.h>


struct KmsDisplay
{
	kms::Connector* m_conn;
	kms::Crtc* m_crtc;
	kms::Plane* m_primary;
};


class KmsManager : public QObject, public kms::PageFlipHandlerBase
{
	Q_OBJECT
public:
	explicit KmsManager(kms::Card& card);

	// PageFlipHandlerBase
	void handle_page_flip(uint32_t frame, double time);

	QList<KmsDisplay> m_displays;
signals:

public slots:

private:
	void drm_event();
	void call_handlers();

	kms::Card& m_card;

	QSocketNotifier* m_sockNotifier;
};

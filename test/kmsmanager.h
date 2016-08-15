#pragma once

#include <QObject>
#include <QSocketNotifier>

#include <kms++/kms++.h>

class KmsManager : public QObject, public kms::PageFlipHandlerBase
{
	Q_OBJECT
public:
	explicit KmsManager(QObject *parent, kms::Card& card);

	// PageFlipHandlerBase
	void handle_page_flip(uint32_t frame, double time);
signals:

public slots:

private:
	void drm_event();
	void call_handlers();

	kms::Card& m_card;

	QSocketNotifier* m_sockNotifier;
};

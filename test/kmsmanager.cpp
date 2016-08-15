#include "kmsmanager.h"

#include <kms++util/kms++util.h>

using namespace kms;

KmsManager::KmsManager(QObject *parent, Card& card)
	: QObject(parent), m_card(card)
{
	m_sockNotifier = new QSocketNotifier(m_card.fd(), QSocketNotifier::Read);
	connect(m_sockNotifier, &QSocketNotifier::activated, this, &KmsManager::drm_event);
	m_sockNotifier->setEnabled(true);
}

void KmsManager::drm_event()
{
	m_card.call_page_flip_handlers();
}

void KmsManager::handle_page_flip(uint32_t frame, double time)
{

}

void KmsManager::call_handlers()
{
	AtomicReq req(m_card);


	int r = req.commit((PageFlipHandlerBase*)this);
	ASSERT(r == 0);
}

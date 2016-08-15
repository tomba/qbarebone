#include "kmsmanager.h"

#include <kms++util/kms++util.h>

using namespace kms;


static void setup_kms_display(KmsDisplay* display, const char* name, ResourceManager& resman)
{
	PixelFormat format = PixelFormat::XRGB8888;

	Connector* conn = resman.reserve_connector(name);
	ASSERT(conn);

	Crtc* crtc = resman.reserve_crtc(conn);
	ASSERT(crtc);

	Plane* primary = resman.reserve_primary_plane(crtc, format);

	display->m_conn = conn;
	display->m_crtc = crtc;
	display->m_primary = primary;
}


KmsManager::KmsManager(Card& card)
	: QObject(), m_card(card)
{
	ResourceManager resman(m_card);

	setup_kms_display(&m_displays[0], "Unknown", resman);
	setup_kms_display(&m_displays[1], "HDMI", resman);

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


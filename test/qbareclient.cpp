#include "qbareclient.h"
#include <QApplication>
#include <QPluginLoader>
#include <QScreen>
#include <qpa/qplatformnativeinterface.h>
#include <QtPlatformSupport/private/qevdevmousemanager_p.h>
#include <QtPlatformSupport/private/qevdevkeyboardmanager_p.h>
#include <QtPlatformSupport/private/qevdevtouchmanager_p.h>
#include <qpa/qwindowsysteminterface.h>
#include <QPainter>

#include "qbareinterface.h"
#include "qbareclientinterface.h"

#include <assert.h>
#include <cstdio>
#include <kms++/kms++.h>
#include <kms++util/kms++util.h>

using namespace kms;

QBareClient::QBareClient(QApplication& a)
{
	QPlatformNativeInterface* native = a.platformNativeInterface();
	ASSERT(native);
	QBareInterface* bare = (QBareInterface*)native->nativeResourceForIntegration("main");
	ASSERT(bare);


	bare->install_client(this);


	m_card = new Card();

	Connector* conn = m_card->get_first_connected_connector();
	ASSERT(conn);

	Crtc* crtc = conn->get_current_crtc();
	ASSERT(crtc);

	const unsigned num_fbs = 2;

#if !use_crtc
	auto mode = conn->get_default_mode();
	uint32_t w = mode.hdisplay;
	uint32_t h = mode.vdisplay;

	for (unsigned i = 0; i < num_fbs; ++i) {
		DumbFramebuffer* fb = new DumbFramebuffer(*m_card, w, h, PixelFormat::XRGB8888);
		draw_test_pattern(*fb);
		m_free_fbs.push_back(fb);
	}

	{
		auto fb = m_free_fbs.back();
		m_free_fbs.pop_back();
		int r = crtc->set_mode(conn, *fb, mode);
		ASSERT(r == 0);
		m_display_fb = fb;
	}

	m_conn = conn;
	m_crtc = crtc;
#else
	auto mode = crtc->mode();

	uint32_t w = mode.hdisplay / 2;
	uint32_t h = mode.vdisplay / 2;

	Plane* plane = 0;

	for (Plane* p : crtc->get_possible_planes())
	{
		if (p->plane_type() != PlaneType::Overlay)
			continue;

		plane = p;
	}

	m_fb = new DumbFramebuffer(*m_card, w, h, PixelFormat::XRGB8888);

	crtc->set_plane(plane, *m_fb,
			0, 0, w, h,
			0, 0, w, h);
#endif

	m_screen = bare->add_screen(QSize(w, h));


	//m_keyManager = new QEvdevKeyboardManager(QLatin1String("EvdevKeyboard"), QString(), this);
	m_mouseManager = new QEvdevMouseManager(QLatin1String("EvdevMouse"), QString(), this);
	//new QEvdevTouchManager("EvdevTouch", QString(), this);

	m_sockNotifier = new QSocketNotifier(m_card->fd(), QSocketNotifier::Read);
	connect(m_sockNotifier, &QSocketNotifier::activated, this, &QBareClient::drmEvent);
	m_sockNotifier->setEnabled(true);
}

void QBareClient::test()
{
	printf("CLIENT TTEST\n");
}

void QBareClient::drmEvent()
{
	m_card->call_page_flip_handlers();
}

void QBareClient::flush()
{
	if (m_free_fbs.size() == 0) {
		m_pending_draw = true;
		return;
	}

	DumbFramebuffer* fb = m_free_fbs.back();
	m_free_fbs.pop_back();

	QImage image(fb->map(0), fb->width(), fb->height(), fb->stride(0), QImage::Format::Format_ARGB32);

	m_screen->draw(image);

	if (m_queued_fb == nullptr) {
		m_crtc->page_flip(*fb, (PageFlipHandlerBase*)this);
		m_queued_fb = fb;
	} else {
		m_ready_fbs.push_back(fb);
	}

	m_pending_draw = false;
}

void QBareClient::handle_page_flip(uint32_t frame, double time)
{
	//printf("page flip, ready: %lu\n", m_ready_fbs.size());

	if (m_display_fb)
		m_free_fbs.push_back(m_display_fb);
	m_display_fb = m_queued_fb;
	m_queued_fb = nullptr;

	if (m_pending_draw)
		flush();

	if (m_ready_fbs.size() == 0)
		return;

	auto fb = m_ready_fbs.front();
	m_ready_fbs.erase(m_ready_fbs.begin());

	m_crtc->page_flip(*fb, (PageFlipHandlerBase*)this);
	m_queued_fb = fb;
}

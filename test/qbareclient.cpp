#include "qbareclient.h"
#include <QApplication>
#include <QPluginLoader>
#include <QScreen>
#include <QPainter>
#include <QImage>

#include "qbareinterface.h"

#include <qpa/qplatformnativeinterface.h>
#include <QtPlatformSupport/private/qevdevmousemanager_p.h>
#include <QtPlatformSupport/private/qevdevkeyboardmanager_p.h>
#include <QtPlatformSupport/private/qevdevtouchmanager_p.h>
#include <QtPlatformSupport/private/qlibinputhandler_p.h>
#include <QtPlatformSupport/private/qfbvthandler_p.h>

#include <unistd.h>
#include <assert.h>
#include <cstdio>
#include <kms++/kms++.h>
#include <kms++util/kms++util.h>

using namespace kms;

static const bool use_crtc = true;
static const bool use_libinput = true;

QBareClient::QBareClient(QApplication& a)
{
	printf("QBareClient()\n");

	ASSERT(a.platformName() == "barebone");

	QPlatformNativeInterface* native = a.platformNativeInterface();
	ASSERT(native);
	QBareInterface* bare = (QBareInterface*)native->nativeResourceForIntegration("main");
	ASSERT(bare);

	bare->install_client(this);

	//new QFbVtHandler(this);

	m_card = new Card();

	m_conn = m_card->get_first_connected_connector();
	ASSERT(m_conn);

	m_crtc = m_conn->get_current_crtc();
	ASSERT(m_crtc);

	const unsigned num_fbs = 2;

	uint32_t w, h;
	Videomode mode;

	if (use_crtc) {
		mode = m_conn->get_default_mode();
		w = mode.hdisplay;
		h = mode.vdisplay;
	} else {
		mode = m_crtc->mode();

		w = mode.hdisplay / 2;
		h = mode.vdisplay / 2;

		for (Plane* p : m_crtc->get_possible_planes())
		{
			if (p->plane_type() != PlaneType::Overlay)
				continue;

			m_plane = p;
		}

		ASSERT(m_plane);
	}

	for (unsigned i = 0; i < num_fbs; ++i) {
		DumbFramebuffer* fb = new DumbFramebuffer(*m_card, w, h, PixelFormat::XRGB8888);
		draw_test_pattern(*fb);
		m_free_fbs.push_back(fb);
	}

	{
		auto fb = m_free_fbs.back();
		m_free_fbs.pop_back();

		if (use_crtc) {
			int r = m_crtc->set_mode(m_conn, *fb, mode);
			ASSERT(r == 0);
		} else {
			int r = m_crtc->set_plane(m_plane, *fb,
						  0, 0, w, h,
						  0, 0, w, h);
			ASSERT(r == 0);
		}

		m_display_fb = fb;
	}

	m_screen = bare->add_screen(QSize(w, h));

	if (use_libinput) {
		new QLibInputHandler(QLatin1String("libinput"), QString());
	} else {
		new QEvdevKeyboardManager(QLatin1String("EvdevKeyboard"), QString(), this);
		new QEvdevMouseManager(QLatin1String("EvdevMouse"), QString(), this);
		new QEvdevTouchManager(QLatin1String("EvdevTouch"), QString(), this);
	}

	m_sockNotifier = new QSocketNotifier(m_card->fd(), QSocketNotifier::Read);
	connect(m_sockNotifier, &QSocketNotifier::activated, this, &QBareClient::drmEvent);
	m_sockNotifier->setEnabled(true);
}

void QBareClient::drmEvent()
{
	m_card->call_page_flip_handlers();
}

void QBareClient::flush()
{
	if (use_crtc) {
		if (m_free_fbs.size() == 0) {
			m_pending_draw = true;
			return;
		}

		DumbFramebuffer* fb = m_free_fbs.back();
		m_free_fbs.pop_back();

		QImage image(fb->map(0), fb->width(), fb->height(), fb->stride(0), QImage::Format::Format_ARGB32);

		m_screen->draw(image);

		if (m_queued_fb == nullptr) {
			int r = m_crtc->page_flip(*fb, (PageFlipHandlerBase*)this);
			ASSERT(r == 0);

			m_queued_fb = fb;
		} else {
			m_ready_fbs.push_back(fb);
		}

		m_pending_draw = false;
	} else {
		DumbFramebuffer* fb = m_free_fbs.back();
		m_free_fbs.pop_back();

		QImage image(fb->map(0), fb->width(), fb->height(), fb->stride(0), QImage::Format::Format_ARGB32);

		m_screen->draw(image);

		uint32_t w = fb->width();
		uint32_t h = fb->height();

		m_crtc->set_plane(m_plane, *fb,
				  0, 0, w, h,
				  0, 0, w, h);

		if (m_display_fb)
			m_free_fbs.push_back(m_display_fb);

		m_display_fb = fb;
	}
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

	int r = m_crtc->page_flip(*fb, (PageFlipHandlerBase*)this);
	ASSERT(r == 0);

	m_queued_fb = fb;
}

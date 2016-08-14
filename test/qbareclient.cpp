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
#include <memory>

using namespace std;
using namespace kms;

static const bool use_libinput = true;

static void setup_kms_display(QKmsDisplay* display, const char* name, ResourceManager& resman)
{
	PixelFormat format = PixelFormat::XRGB8888;

	Card& card = resman.card();

	Connector* conn = resman.reserve_connector(name);
	ASSERT(conn);

	Crtc* crtc = resman.reserve_crtc(conn);
	ASSERT(crtc);

	Plane* primary = resman.reserve_primary_plane(crtc, format);

	display->m_conn = conn;
	display->m_crtc = crtc;
	display->m_primary = primary;

	const unsigned num_fbs = 2;

	Videomode mode = conn->get_default_mode();
	uint32_t w = mode.hdisplay;
	uint32_t h = mode.vdisplay;

	// Allocate buffers
	for (unsigned i = 0; i < num_fbs; ++i) {
		DumbFramebuffer* fb = new DumbFramebuffer(card, w, h, format);
		draw_test_pattern(*fb);
		display->m_free_fbs.push_back(fb);
	}

	// Setup initial display

	DumbFramebuffer* fb = display->m_free_fbs.back();
	display->m_free_fbs.pop_back();

	display->m_display_fb = fb;

	AtomicReq req(card);

	unique_ptr<Blob> mode_blob = mode.to_blob(card);

	req.add_display(conn, crtc, mode_blob.get(), primary, fb);

	int r = req.commit_sync(true);
	ASSERT(r == 0);
}

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

	ResourceManager resman(*m_card);

	setup_kms_display(&m_lcd, "Unknown", resman);
	setup_kms_display(&m_hdmi, "HDMI", resman);

	m_lcd.m_screen = bare->add_screen(QSize(m_lcd.m_display_fb->width(), m_lcd.m_display_fb->height()), "LCD");
	m_hdmi.m_screen = bare->add_screen(QSize(m_hdmi.m_display_fb->width(), m_hdmi.m_display_fb->height()), "HDMI");

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

void QBareClient::flush(QBareScreenInterface* screen)
{
	if (screen == m_lcd.m_screen)
		m_lcd.flush();
	else if (screen == m_hdmi.m_screen)
		m_hdmi.flush();
}

void QKmsDisplay::flush()
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
		AtomicReq req(m_crtc->card());
		req.add(m_primary, { { "FB_ID", fb->id() } });
		int r = req.commit((PageFlipHandlerBase*)this);
		ASSERT(r == 0);

		m_queued_fb = fb;
	} else {
		m_ready_fbs.push_back(fb);
	}

	m_pending_draw = false;
}

void QKmsDisplay::handle_page_flip(uint32_t frame, double time)
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

	AtomicReq req(m_crtc->card());
	req.add(m_primary, { { "FB_ID", fb->id() } });
	int r = req.commit((PageFlipHandlerBase*)this);
	ASSERT(r == 0);

	m_queued_fb = fb;
}

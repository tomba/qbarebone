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

#if !use_crtc
	auto mode = conn->get_default_mode();
	uint32_t w = mode.hdisplay;
	uint32_t h = mode.vdisplay;
	m_fb = new DumbFramebuffer(*m_card, w, h, PixelFormat::XRGB8888);
	draw_test_pattern(*m_fb);
	int r = crtc->set_mode(conn, *m_fb, mode);
	ASSERT(r == 0);

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

	m_image = QImage(m_fb->map(0), m_fb->width(), m_fb->height(), m_fb->stride(0), QImage::Format::Format_ARGB32);

	m_screen = bare->add_screen(QSize(w, h));
	m_screen->setBackbuffer(&m_image);


	//m_keyManager = new QEvdevKeyboardManager(QLatin1String("EvdevKeyboard"), QString(), this);
	m_mouseManager = new QEvdevMouseManager(QLatin1String("EvdevMouse"), QString(), this);
	//new QEvdevTouchManager("EvdevTouch", QString(), this);
}

void QBareClient::test()
{
	printf("CLIENT TTEST\n");
}

void QBareClient::flush()
{
	//printf("flush: %u\n", m_cur_fb);

	return m_fb->flush();

}

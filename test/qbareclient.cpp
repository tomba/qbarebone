#include "qbareclient.h"
#include <QApplication>
#include <QPluginLoader>
#include <QScreen>
#include <qpa/qplatformnativeinterface.h>
#include <QtPlatformSupport/private/qevdevmousemanager_p.h>
#include <QtPlatformSupport/private/qevdevkeyboardmanager_p.h>
#include <qpa/qwindowsysteminterface.h>

#include "qbareinterface.h"
#include "qbareclientinterface.h"

#include <assert.h>
#include <cstdio>
#include <kms++/kms++.h>

using namespace kms;

QBareClient::QBareClient(QApplication& a)
{
	QPlatformNativeInterface* native = a.platformNativeInterface();
	assert(native);
	QBareInterface* bare = (QBareInterface*)native->nativeResourceForIntegration("main");
	assert(bare);


	bare->install_client(this);


	m_card = new Card();

	Connector* conn = m_card->get_first_connected_connector();
	assert(conn);

	Crtc* crtc = conn->get_current_crtc();
	assert(crtc);

#if use_crtc
	auto mode = conn->get_default_mode();
	uint32_t w = mode.hdisplay;
	uint32_t h = mode.vdisplay;
	m_fb = new DumbFramebuffer(*m_card, w, h, PixelFormat::XRGB8888);
	crtc->set_mode(conn, *m_fb, mode);
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

	bare->add_screen(QSize(w, h));


	m_keyManager = new QEvdevKeyboardManager(QLatin1String("EvdevKeyboard"), QString(), this);
	m_mouseManager = new QEvdevMouseManager(QLatin1String("EvdevMouse"), QString(), this);
}

void QBareClient::test()
{
	printf("CLIENT TTEST\n");
}

QImage QBareClient::get_qimage(const QSize& size)
{
	printf("GET QIMAGE %dx%d\n", size.width(), size.height());

	return QImage(m_fb->map(0), m_fb->width(), m_fb->height(), m_fb->stride(0), QImage::Format::Format_ARGB32);
}

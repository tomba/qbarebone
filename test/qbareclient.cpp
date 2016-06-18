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

#include "cube-egl.h"
#include "cube-gbm.h"

#include <gbm.h>

using namespace kms;

QBareClient::QBareClient(QApplication& a)
{
	QPlatformNativeInterface* native = a.platformNativeInterface();
	assert(native);
	QBareInterface* bare = (QBareInterface*)native->nativeResourceForIntegration("main");
	assert(bare);

	bare->install_client(this);


	m_card = new Card();
	m_gbm = new GbmDevice(*m_card);
	m_eglState = new EglState(m_gbm);



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
#elif use_plane
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

	m_surface = new GbmEglSurface(*m_card, *m_gbm, *m_eglState, w, h);
	m_bo = m_surface->lock_next_bo();
	Framebuffer* fb = m_surface->drm_fb_get_from_bo(m_bo, *m_card);

	crtc->set_plane(plane, *fb,
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

	uint32_t w = gbm_bo_get_width(m_bo);
	uint32_t h = gbm_bo_get_height(m_bo);
	uint32_t stride = gbm_bo_get_stride(m_bo);
	//gbm_bo_

	//return QImage(m_fb->map(0), w, h, stride, QImage::Format::Format_ARGB32);
	return QImage();
}

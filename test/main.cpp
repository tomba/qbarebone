#include "mainwindow.h"
#include <QApplication>
#include <QPluginLoader>
#include <QScreen>
#include <qpa/qplatformnativeinterface.h>
#include <QtPlatformSupport/private/qevdevmousemanager_p.h>
#include <QtPlatformSupport/private/qevdevkeyboardmanager_p.h>
#include <qpa/qwindowsysteminterface.h>

#include "qbareinterface.h"
#include "qbareclientinterface.h"

#include <cstdio>
#include "kms++.h"

using namespace kms;

class BB : public QObject, public QBareClientInterface
{
	Q_OBJECT

public:
	BB(QApplication& a)
	{
		m_card = new Card();

		QPlatformNativeInterface *native = a.platformNativeInterface();
		QBareInterface* bare = (QBareInterface*)native->nativeResourceForIntegration("main");


		bare->install_client(this);



		Connector* conn = m_card->get_first_connected_connector();

		if(!conn)
			printf("No connector\n");

		printf("Adding KMS crtc+connector %s\n", conn->fullname().c_str());

		Crtc* crtc = conn->get_possible_crtcs().front();

#if use_crtc
		auto mode = conn->get_default_mode();
		uint32_t w = mode.hdisplay;
		uint32_t h = mode.vdisplay;
		m_fb = new DumbFramebuffer(*m_card, w, h, PixelFormat::XRGB8888);
		crtc->set_mode(conn, *m_fb, mode);
#else
		uint32_t w = 800;
		uint32_t h = 800;

		Plane* plane = 0;

		for (Plane* p : crtc->get_possible_planes())
		{
			if (p->plane_type() != PlaneType::Overlay)
				continue;

			plane = p;
		}

		m_fb = new DumbFramebuffer(*m_card, 800, 800, PixelFormat::XRGB8888);

		crtc->set_plane(plane, *m_fb, 0, 0, 800, 800, 0, 0, 800, 800);
#endif

		bare->add_screen(QSize(w, h));


		m_keyManager = new QEvdevKeyboardManager(QLatin1String("EvdevKeyboard"), QString(), this);
		m_mouseManager = new QEvdevMouseManager(QLatin1String("EvdevMouse"), QString(), this);
	}

	void test()
	{
		printf("CLIENT TTEST\n");
	}

	virtual QImage get_qimage(const QSize& size)
	{
		printf("GET QIMAGE %dx%d\n", size.width(), size.height());

		return QImage(m_fb->map(0), m_fb->width(), m_fb->height(), m_fb->stride(0), QImage::Format::Format_ARGB32);
	}

private:
	Card* m_card;
	DumbFramebuffer* m_fb;
	QEvdevKeyboardManager* m_keyManager;
	QEvdevMouseManager* m_mouseManager;
};

#include "main.moc"

int main(int argc, char *argv[])
{
	QApplication::addLibraryPath(".");

/*
	QApplication::addLibraryPath("./platforms");
	QPluginLoader loader("barebone");
	if (!loader.load()) {
		printf("load failed\n");
		return 0;
	}
*/

	printf("Create App\n");
	QApplication a(argc, argv);

	BB* bb;

	if (a.platformName() == "barebone") {
		printf("Create BB\n");
		bb = new BB(a);
	}

	printf("Create window\n");
	MainWindow w;
	w.show();

	printf("Enter mainloop\n");
	int r = a.exec();

	if (bb)
		delete bb;

	return r;
}

#include "mainwindow.h"
#include <QApplication>
#include <QPluginLoader>
#include <QScreen>
#include <qpa/qplatformnativeinterface.h>

#include "qbareinterface.h"
#include "qbareclientinterface.h"

#include <cstdio>
#include "kms++.h"

using namespace kms;

class BB : public QBareClientInterface
{
public:
	BB(QApplication& a)
	{
		m_card = new Card();

		QPlatformNativeInterface *native = a.platformNativeInterface();
		QBareInterface* bare = (QBareInterface*)native->nativeResourceForIntegration("main");

		for (const auto& pipe : m_card->get_connected_pipelines()) {
			printf("Adding KMS crtc+connector %s\n", pipe.connector->fullname().c_str());

			uint32_t w = pipe.crtc->width();
			uint32_t h = pipe.crtc->height();

			bare->add_screen(QSize(w, h));

			m_fb = new DumbFramebuffer(*m_card, w, h, PixelFormat::ABGR8888);

			pipe.crtc->set_mode(pipe.connector, *m_fb, pipe.crtc->mode());

			bare->install_client(this);
		}
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
};

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

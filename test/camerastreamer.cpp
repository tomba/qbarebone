#include "camerastreamer.h"

using namespace kms;

CameraStreamer::CameraStreamer(QObject *parent, VideoStreamer* streamer, Plane* plane, uint32_t width, uint32_t height, PixelFormat pixfmt)
	: QObject(parent), m_capdev(*streamer), m_plane(plane)
{
	const unsigned queue_size = 5;

	for (unsigned i = 0; i < queue_size; ++i) {
		auto fb = new DumbFramebuffer(plane->card(), width, height, pixfmt);
		m_fbs.append(fb);
		m_free_fbs.enqueue(fb);
	}

	m_capdev.set_port(0);
	m_capdev.set_format(pixfmt, width, height);
	m_capdev.set_queue_size(m_fbs.size());

	while (DumbFramebuffer* fb = m_free_fbs.dequeue())
		m_capdev.queue(fb);

	m_sockNotifier = new QSocketNotifier(m_capdev.fd(), QSocketNotifier::Read);
	connect(m_sockNotifier, &QSocketNotifier::activated, this, &CameraStreamer::cameraEvent);
	m_sockNotifier->setEnabled(true);
}

CameraStreamer::~CameraStreamer()
{
	for (auto fb : m_fbs)
		delete fb;
}

int CameraStreamer::fd() const { return m_capdev.fd(); }

void CameraStreamer::start_streaming()
{
	m_capdev.stream_on();
}

void CameraStreamer::stop_streaming()
{
	m_capdev.stream_off();
}

void CameraStreamer::cameraEvent()
{
	DumbFramebuffer* fb = m_capdev.dequeue();
	m_ready_fbs.enqueue(fb);

	emit received_buffer();
}

void CameraStreamer::handle_page_flip()
{
	if (m_display_fb)
		m_free_fbs.enqueue(m_display_fb);
	m_display_fb = m_queued_fb;
	m_queued_fb = nullptr;
}

bool CameraStreamer::setup_page_flip(AtomicReq& req)
{
	if (m_queued_fb)
		return false;

	if (m_ready_fbs.empty())
		return false;

	DumbFramebuffer* fb = m_ready_fbs.dequeue();

	req.add(m_plane, { { "FB_ID", fb->id() } });

	m_queued_fb = fb;

	return true;
}

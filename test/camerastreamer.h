#pragma once

#include <QObject>
#include <QSocketNotifier>

#include <kms++/kms++.h>
#include <kms++util/kms++util.h>
#include <kms++util/videodevice.h>

#include <QList>
#include <QQueue>

class CameraStreamer : public QObject
{
	Q_OBJECT
public:
	CameraStreamer(QObject* parent, VideoStreamer* streamer, kms::Plane* plane, uint32_t width, uint32_t height, kms::PixelFormat pixfmt);
	~CameraStreamer();

	CameraStreamer(const CameraStreamer& other) = delete;
	CameraStreamer& operator=(const CameraStreamer& other) = delete;

	int fd() const;

	void start_streaming();
	void stop_streaming();

	void handle_page_flip();

	bool setup_page_flip(kms::AtomicReq& req);

signals:
	void received_buffer();

public slots:

private:
	void cameraEvent();

	VideoStreamer& m_capdev;

	kms::Plane* m_plane;

	QList<kms::DumbFramebuffer*> m_fbs;
	QQueue<kms::DumbFramebuffer*> m_free_fbs;
	QQueue<kms::DumbFramebuffer*> m_ready_fbs;

	kms::DumbFramebuffer* m_queued_fb;
	kms::DumbFramebuffer* m_display_fb;

	QSocketNotifier* m_sockNotifier;
};

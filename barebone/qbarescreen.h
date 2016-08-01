#pragma once

#include <qpa/qplatformscreen.h>
#include "qbarecursor.h"
#include "qbareinterface.h"

class QBareIntegration;
class QBareWindow;

class QBareScreen : public QObject, public QPlatformScreen, public QBareScreenInterface
{
	Q_OBJECT
public:
	QBareScreen(QString name, const QRect& geom, int depth, QImage::Format format, QBareIntegration* integration);

	// QPlatformScreen
	QRect geometry() const { return m_geometry; }
	int depth() const { return m_depth; }
	QImage::Format format() const { return m_format; }
	QString name() const { return m_name; }

	QPlatformCursor *cursor() const;

	// QBareScreenInterface
	void draw(QImage& qimage);

	void present();

	void addWindow(QBareWindow* wnd);
	void removeWindow(QBareWindow* wnd);

	void scheduleUpdate();

	void raise(QBareWindow* wnd);
	void lower(QBareWindow* wnd);

protected:
	bool event(QEvent *event) Q_DECL_OVERRIDE;

private:
	QString m_name;

	QRect m_geometry;
	int m_depth;
	QImage::Format m_format;
	QSize m_physicalSize;
	QBareCursor* m_cursor;
	QBareIntegration* m_integration;

	QList<QBareWindow*> m_windowStack;

	bool m_updatePending;
};


#include <qpa/qplatformintegrationplugin.h>
#include "qbareintegration.h"

#include <cstdio>

QT_BEGIN_NAMESPACE

class QMinimalIntegrationPlugin : public QPlatformIntegrationPlugin
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QPA.QPlatformIntegrationFactoryInterface.5.2" FILE "barebone.json")
public:
	QPlatformIntegration *create(const QString&, const QStringList&);
};

QPlatformIntegration *QMinimalIntegrationPlugin::create(const QString& system, const QStringList& paramList)
{
	if (!system.compare(QLatin1String("barebone"), Qt::CaseInsensitive))
		return new QBareIntegration(paramList);

	return 0;
}

QT_END_NAMESPACE

#include "main.moc"

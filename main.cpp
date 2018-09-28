#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlComponent>

#include "backend.h"
#include "rep_remoteeventhandler_replica.h"
#include "rep_remotebackend_replica.h"
#include "remoteeventhandler.h"
#include "platformcontrolmock.h"
#include "platformcontrolrpmsg.h"
#include "sensordata.h"

int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    if (qEnvironmentVariableIsEmpty("QTGLESSTREAM_DISPLAY")) {
        qputenv("QT_QPA_EGLFS_PHYSICAL_WIDTH", QByteArray("213"));
        qputenv("QT_QPA_EGLFS_PHYSICAL_HEIGHT", QByteArray("120"));

        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    }

    // Allow SensorData in queued connections
    qRegisterMetaType<SensorData>();

    QApplication app(argc, argv);

    QScopedPointer<QObject> backend;

    QQmlApplicationEngine engine;

    QRemoteObjectNode *remoteClient;
    RemoteEventHandler remoteEventHandler;

    // qmlRegisterType<SensorData>("com.toradex.examples.sensordata", 1, 0, "SensorData");
    qmlRegisterType<BackEnd>("com.toradex.examples.backend", 1, 0, "RemoteBackEnd");

    if (app.platformName() == QLatin1String("webgl")) {
        RemoteBackendReplica *replica;
        remoteClient = new QRemoteObjectNode(&app);
        remoteClient->connectToNode(QUrl("local:backendServer"));
        replica = remoteClient->acquire<RemoteBackendReplica>();

        /* Process the events, so that the the replica is initialized before we start the GUI */
        while(!replica->isInitialized())
            app.processEvents();

        backend.reset(replica);
    } else {
        /* Switch mock/real implementation */
//        PlatformControlRpmsg *controller = new PlatformControlRpmsg(&app);
//        controller->initialize();
        PlatformControlMock *controller = new PlatformControlMock(&app);
        backend.reset(new BackEnd(controller));
    }

    engine.rootContext()->setContextProperty("backend", backend.data());
    engine.rootContext()->setContextProperty("remoteEventHandler", &remoteEventHandler);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}

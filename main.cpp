#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlComponent>

#include "backend.h"
#include "remotebackendwrapper.h"
#include "remoteeventhandler.h"
#include "platformcontrolmock.h"

int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    if (qEnvironmentVariableIsEmpty("QTGLESSTREAM_DISPLAY")) {
        qputenv("QT_QPA_EGLFS_PHYSICAL_WIDTH", QByteArray("213"));
        qputenv("QT_QPA_EGLFS_PHYSICAL_HEIGHT", QByteArray("120"));

        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    }

    QApplication app(argc, argv);

    QScopedPointer<BackEndInterface> backend;

    RemoteEventHandler remoteEventHandler;
    QQmlApplicationEngine engine;
    RemoteBackEndWrapper *backendWrapper;
    RemoteBackendReplica *replica;
    QRemoteObjectNode *remoteClient;

    qmlRegisterType<BackEnd>("com.toradex.examples.backend", 1, 0, "RemoteBackEnd");

    if (app.platformName() == QLatin1String("webgl")) {
        remoteClient = new QRemoteObjectNode(&app);
        remoteClient->connectToNode(QUrl("local:backendServer"));
        replica = remoteClient->acquire<RemoteBackendReplica>();

        while(!replica->isInitialized())
            app.processEvents();

        backendWrapper = new RemoteBackEndWrapper(replica);
        backend.reset(backendWrapper);
    } else {
        PlatformControl *controller = new PlatformControlMock(&app);
        backend.reset(new BackEnd(controller));
    }

    engine.rootContext()->setContextProperty("backend", backend.data());
    engine.rootContext()->setContextProperty("remoteEventHandler", &remoteEventHandler);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}

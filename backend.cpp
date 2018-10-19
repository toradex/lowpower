#include <QDebug>
#include <QtCharts/QLineSeries>
#include <QNetworkInterface>
#include <QCoreApplication>

#include <cmath>

#include "backend.h"

#define STORAGE_DEPTH 101

BackEnd::BackEnd(PlatformControl *controller, QObject *parent) : QObject(parent),
    m_accelerationData(3, STORAGE_DEPTH), m_gyroData(3, STORAGE_DEPTH), m_magnetoData(3, STORAGE_DEPTH), m_powerData(1, STORAGE_DEPTH),
    m_controller(*controller)
{
    connect(&m_controller, SIGNAL(dataReceived(const QByteArray &)), this, SLOT(processData(const QByteArray &)));
}

static qreal convertData(char byte1, char byte2, qreal divider)
{
    quint16 value;
    qint16 svalue;
    value = static_cast<quint16>((static_cast<quint16>(byte1) << 0) + (static_cast<quint16>(byte2) << 8));
    svalue = static_cast<qint16>(value);
    return static_cast<qreal>(svalue)/divider;
}

void BackEnd::processData(const QByteArray &data)
{
#define GYRO_DIVIDER 8
#define ACC_DIVIDER (8192.0/9.81)
#define MAG_DIVIDER 10
#define POWER_DIVIDER 8
#define CURRENT_DIVIDER 10000

    if (data.size() != 24) {
        qWarning() << "process data must be exactly 24 bytes but is " << data.size();
    }

    if (0xFF != static_cast<quint8>(data[0])) {
        qWarning() << "process data must start with 0xFF";
        return;
    }

    /* Do some uggly conversion as done int the original, should maybe fixed later */
    m_gyroData.push_pop(0, std::fmod(convertData(data[1], data[2], GYRO_DIVIDER) + 180, 360) - 180);
    m_gyroData.push_pop(1, std::fmod(convertData(data[3], data[4], GYRO_DIVIDER) + 180, 360) - 180);
    m_gyroData.push_pop(2, std::fmod(convertData(data[5], data[6], GYRO_DIVIDER) + 180, 360) - 180);

    m_accelerationData.push_pop(0, convertData(data[7], data[8], ACC_DIVIDER));
    m_accelerationData.push_pop(1, convertData(data[9], data[10], ACC_DIVIDER));
    m_accelerationData.push_pop(2, convertData(data[11], data[12], ACC_DIVIDER));

    m_magnetoData.push_pop(0, convertData(data[13], data[14], MAG_DIVIDER));
    m_magnetoData.push_pop(1, convertData(data[15], data[16], MAG_DIVIDER));
    m_magnetoData.push_pop(2, convertData(data[17], data[18], MAG_DIVIDER));

    qreal fvalue;
    quint16 value;
    value = static_cast<quint16>((static_cast<quint16>(data[20]) << 0) + (static_cast<quint16>(data[21]) << 8));
    fvalue = value/POWER_DIVIDER;
    m_powerData.push_pop(0, fvalue);

    /* Current would be 22 23, we ignore that DIVIDER would be 10000 */

    /* This is dirty */
    emit accelerationDataChanged(m_accelerationData);
    emit gyroDataChanged(m_gyroData);
    emit magnetoDataChanged(m_magnetoData);
    emit powerDataChanged(m_powerData);
}

int BackEnd::accelerationDataAxis() const
{
    return 3;
}

int BackEnd::gyroDataAxis() const
{
    return 3;
}

int BackEnd::magnetoDataAxis() const
{
    return 3;
}

int BackEnd::powerDataAxis() const
{
    return 1;
}

int BackEnd::storageDepth() const
{
    return STORAGE_DEPTH;
}

bool BackEnd::remotingEnabled() const
{
    return !m_remoteAccess.isNull();
}

void BackEnd::setRemotingEnabled(bool enable)
{
    if (enable == remotingEnabled())
        return;

    if (enable) {
        updateServerAddress();

        m_remoteBackend.reset(new ReadOnlyRemoteBackend(this));
        m_backendServer.reset(new QRemoteObjectHost);
        m_backendServer->setHostUrl(QUrl("local:backendServer"));
        m_backendServer->enableRemoting(m_remoteBackend.data());

        QProcess *p = new QProcess(this);
        m_remoteAccess = p;
        QObject::connect(m_remoteAccess, &QProcess::stateChanged, [this] (QProcess::ProcessState state) {
            if (m_remoteAccess && state == QProcess::Running) {
                qDebug() << "Remote control process running, pid:" << m_remoteAccess->processId();
            }
        });
        QObject::connect(m_remoteAccess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                [p](int, QProcess::ExitStatus) {
            qDebug() << "Remote control process stopped";
            p->deleteLater();
        });
        m_remoteAccess->start(QCoreApplication::applicationFilePath(), QStringList() << "-platform" << "webgl", QProcess::ReadOnly);
    } else {
        m_backendServer->disableRemoting(m_remoteBackend.data());
        m_backendServer.reset();
        m_remoteBackend.reset();

        m_remoteAccess->kill();
        m_remoteAccess.clear();
    }

    emit remotingEnabledChanged(enable);
}

void BackEnd::shutdown() const
{
    m_controller.shutdown();
}

void BackEnd::sleep() const
{
    m_controller.sleep();
}

QString BackEnd::test() const
{
    qDebug() << "test";
    return "Test";
}

int BackEnd::tabIndex() const
{
    return m_tabIndex;
}

void BackEnd::setTabIndex(int tabIndex)
{
    m_tabIndex = tabIndex;
    emit tabIndexChanged(tabIndex);
}

SensorData BackEnd::powerData() const
{
    return m_powerData;
}

SensorData BackEnd::magnetoData() const
{
    return m_magnetoData;
}

SensorData BackEnd::gyroData() const
{
    return m_gyroData;
}

SensorData BackEnd::accelerationData() const
{
    return m_accelerationData;
}

void BackEnd::updateServerAddress()
{
    QList<QString> addresses;
    for (const QNetworkInterface &iface : QNetworkInterface::allInterfaces()) {
        if (iface.flags() & (QNetworkInterface::IsUp | QNetworkInterface::IsRunning)) {
            for (const QNetworkAddressEntry &entry : iface.addressEntries()) {
                if (entry.ip().protocol() != QAbstractSocket::IPv4Protocol)
                    continue;

                if (!iface.flags().testFlag(QNetworkInterface::IsLoopBack))
                    addresses.append(entry.ip().toString());
            }
        }
    }

    m_serverAddress = "";
    for (auto address : addresses) {
        m_serverAddress += QString("http://") + address + QString(":8080\n");
    }
    qDebug() << m_serverAddress;
    emit serverAddressChanged();
}

ReadOnlyRemoteBackend::ReadOnlyRemoteBackend(BackEnd *backend, QObject *parent)
    : RemoteBackendSource(parent)
    , m_backend(backend)
{
    connect(m_backend, &BackEnd::accelerationDataChanged, this, &ReadOnlyRemoteBackend::accelerationDataChanged);
    connect(m_backend, &BackEnd::gyroDataChanged, this, &ReadOnlyRemoteBackend::gyroDataChanged);
    connect(m_backend, &BackEnd::magnetoDataChanged, this, &ReadOnlyRemoteBackend::magnetoDataChanged);
    connect(m_backend, &BackEnd::powerDataChanged, this, &ReadOnlyRemoteBackend::powerDataChanged);
    connect(m_backend, &BackEnd::remotingEnabledChanged, this, &ReadOnlyRemoteBackend::remotingEnabledChanged);
    connect(m_backend, &BackEnd::tabIndexChanged, this, &ReadOnlyRemoteBackend::tabIndexChanged);
}

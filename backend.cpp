#include <QDebug>
#include <QtCharts/QXYSeries>
#include <QNetworkInterface>
#include <QCoreApplication>

#include "backend.h"

BackEnd::BackEnd(PlatformControl *controller, QObject *parent) : BackEndInterface(parent), m_controller(*controller)
{
    initializeData(m_accelerationData, 3, storageDepth());
    initializeData(m_gyroData, 3, storageDepth());
    initializeData(m_magnetoData, 3, storageDepth());
    initializeData(m_powerData, 1, storageDepth());

    connect(&m_controller, SIGNAL(dataReceived(const QString &)), this, SLOT(processData(const QString &)));
}

void BackEnd::initializeData(QVector<QVector<QPointF> > &data, int rowCount, int colCount)
{
    // Remove previous data
    data.clear();

    // Append the new data depending on the type
    for (int i(0); i < rowCount; i++) {
        QVector<QPointF> points;
        points.reserve(colCount);
        for (int j(0); j < colCount; j++) {
            qreal x(j);
            qreal y(0);
            points.append(QPointF(x, y));
        }
        data.append(points);
    }
}

void BackEnd::processData(const QString &str)
{

    /* Response from M4 will be a string of form "acc?x?y?z" */
    QStringList list = str.split(',');

    if (list.size() > 4) {
        qWarning() << "List is > 4, this is not handled at the moment";
        return;
    }

    for (int i = 0; i < list.size()-1; i ++) {
        bool ok;
        qreal val = list.at(i + 1).toDouble(&ok);
        if (ok == false) {
            qWarning() << "Invalid value " << val;
            return;
        }

        if (list.at(0) == "acc") {
            m_accelerationData[i].push_front(QPointF(m_accelerationPos, val));
            m_accelerationData[i].pop_back();

        } else if (list.at(0) == "gyro") {
            m_gyroData[i].push_front(QPointF(m_gyroPos, val));
            m_gyroData[i].pop_back();
        } else if (list.at(0) == "mag") {
            m_magnetoData[i].push_front(QPointF(m_magnetoPos, val));
            m_magnetoData[i].pop_back();
        } else if (list.at(0) == "alti") {
            m_powerData[i].push_front(QPointF(m_powerPos, val));
            m_powerData[i].pop_back();
        }
   }

    if (list.at(0) == "acc") {
        m_accelerationPos++;
        m_accelerationPos %= storageDepth();
        emit accelerationDataChanged();
    } else if (list.at(0) == "gyro") {
        m_gyroPos++;
        m_gyroPos %= storageDepth();
        emit gyroDataChanged();
    } else if (list.at(0) == "mag") {
        m_magnetoPos++;
        m_magnetoPos %= storageDepth();
        emit magnetoDataChanged();
    } else if (list.at(0) == "alti") {
        m_powerPos++;
        m_powerPos %= storageDepth();
        emit powerDataChanged();
    }


}

QPointF BackEnd::accelerationData(int sensor, int at) const
{
    return m_accelerationData[sensor][at];
}

int BackEnd::accelerationDataAxis() const
{
    return 3;
}

QPointF BackEnd::gyroData(int sensor, int at) const
{
    return m_gyroData[sensor][at];
}

int BackEnd::gyroDataAxis() const
{
    return 3;
}

QPointF BackEnd::magnetoData(int sensor, int at) const
{
    return m_magnetoData[sensor][at];
}

int BackEnd::magnetoDataAxis() const
{
    return 3;
}

QPointF BackEnd::powerData(int sensor, int at) const
{
    return m_powerData[sensor][at];
}

int BackEnd::powerDataAxis() const
{
    return 1;
}

int BackEnd::storageDepth() const
{
    return 11;
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
        m_remoteAccess->kill();
        m_remoteAccess.clear();

        m_backendServer->disableRemoting(m_remoteBackend.data());
        m_backendServer.reset();
        m_remoteBackend.reset();
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

void BackEnd::updateServerAddress()
{
    QMap<int, QString> addresses;
    for (const QNetworkInterface &iface : QNetworkInterface::allInterfaces()) {
        if (iface.flags() & (QNetworkInterface::IsUp | QNetworkInterface::IsRunning)) {
            for (const QNetworkAddressEntry &entry : iface.addressEntries()) {
                if (entry.ip().protocol() != QAbstractSocket::IPv4Protocol)
                    continue;

                if (iface.humanReadableName().contains(QLatin1String("usb0"), Qt::CaseInsensitive))
                    addresses.insert(1, entry.ip().toString());
                else if (iface.humanReadableName().contains(QLatin1String("ethernet"), Qt::CaseInsensitive)
                         || iface.humanReadableName().contains(QLatin1String("eth0"), Qt::CaseInsensitive))
                    addresses.insert(2, entry.ip().toString());
                else if (iface.humanReadableName().contains(QLatin1String("wi-fi"), Qt::CaseInsensitive))
                    addresses.insert(3, entry.ip().toString());
                else if (iface.flags().testFlag(QNetworkInterface::IsLoopBack))
                    addresses.insert(4, entry.ip().toString());
            }
        }
    }

    m_serverAddress = addresses.first();
    emit serverAddressChanged();
}

ReadOnlyRemoteBackend::ReadOnlyRemoteBackend(const BackEnd *backend, QObject *parent)
    : RemoteBackendSource(parent)
    , m_backend(backend)
{
    connect(m_backend, &BackEnd::accelerationDataChanged, this, &ReadOnlyRemoteBackend::accelerationDataChanged);
    connect(m_backend, &BackEnd::gyroDataChanged, this, &ReadOnlyRemoteBackend::gyroDataChanged);
    connect(m_backend, &BackEnd::magnetoDataChanged, this, &ReadOnlyRemoteBackend::magnetoDataChanged);
    connect(m_backend, &BackEnd::powerDataChanged, this, &ReadOnlyRemoteBackend::powerDataChanged);
}

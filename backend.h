#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QVector>
#include <QPointF>
#include <QPointer>
#include <QProcess>
#include <QRemoteObjectHost>
#include <QtCharts/QAbstractSeries>
#include <QtCharts/QXYSeries>

#include "rep_remotebackend_source.h"
#include "platformcontrol.h"
#include "sensordata.h"

class ReadOnlyRemoteBackend;

class BackEnd : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString serverAddress READ serverAddress NOTIFY serverAddressChanged)
    Q_PROPERTY(bool remotingEnabled READ remotingEnabled WRITE setRemotingEnabled NOTIFY remotingEnabledChanged)
    Q_PROPERTY(bool remoteControlEnabled READ remoteControlEnabled WRITE setRemoteControlEnabled NOTIFY remoteControlEnabledChanged)
    Q_PROPERTY(SensorData accelerationData READ accelerationData NOTIFY accelerationDataChanged)
    Q_PROPERTY(SensorData gyroData READ gyroData NOTIFY gyroDataChanged)
    Q_PROPERTY(SensorData magnetoData READ magnetoData NOTIFY magnetoDataChanged)
    Q_PROPERTY(SensorData powerData READ powerData NOTIFY powerDataChanged)

public:
    explicit BackEnd(PlatformControl *controller = nullptr, QObject *parent = nullptr);

    void initializeData(QVector<QVector<QPointF> > &data, int rowCount, int colCount);

    Q_INVOKABLE int accelerationDataAxis() const;
    Q_INVOKABLE int gyroDataAxis() const;
    Q_INVOKABLE int magnetoDataAxis() const;
    Q_INVOKABLE int powerDataAxis() const;

    Q_INVOKABLE int storageDepth() const;

    SensorData accelerationData() const;
    SensorData gyroData() const;
    SensorData magnetoData() const;
    SensorData powerData() const;

public Q_SLOTS:
    void processData(const QByteArray &data);

    // Remote stuff
    QString serverAddress() const { return m_serverAddress; }

    bool remotingEnabled() const;
    void setRemotingEnabled(bool enable);

    bool remoteControlEnabled() const { return m_remoteControl; }
    void setRemoteControlEnabled(bool enable) {
        m_remoteControl = enable;
        emit remoteControlEnabledChanged(enable);
    }

    void shutdown() const;
    void sleep() const;
    QString test() const;

Q_SIGNALS:
    // Remote stuff
    void remotingEnabledChanged(bool remotingEnabled);
    void serverAddressChanged();
    void remoteControlEnabledChanged(bool remoteControlEnabled);

    void accelerationDataChanged(SensorData sensorData);
    void gyroDataChanged(SensorData sensorData);
    void magnetoDataChanged(SensorData sensorData);
    void powerDataChanged(SensorData sensorData);


private:
    SensorData m_accelerationData;
    SensorData m_gyroData;
    SensorData m_magnetoData;
    SensorData m_powerData;

    PlatformControl &m_controller;

    // Remote stuff
    void updateServerAddress();

    QString m_serverAddress;

    QPointer<QProcess> m_remoteAccess;
    QScopedPointer<QRemoteObjectHost> m_backendServer;
    QScopedPointer<ReadOnlyRemoteBackend> m_remoteBackend;
    bool m_remoteControl;
};

class ReadOnlyRemoteBackend : public RemoteBackendSource
{
    Q_OBJECT
public:
    explicit ReadOnlyRemoteBackend(const BackEnd *backend, QObject *parent = nullptr);

    QString serverAddress() const { return m_backend->serverAddress(); }
    bool remotingEnabled() const { return m_backend->remotingEnabled(); }
    void setRemotingEnabled(bool) { }

    bool remoteControlEnabled() const { return m_backend->remoteControlEnabled(); }
    void setRemoteControlEnabled(bool) { }

    int accelerationDataAxis() { return m_backend->accelerationDataAxis(); }
    int gyroDataAxis() { return m_backend->gyroDataAxis(); }
    int magnetoDataAxis() { return m_backend->magnetoDataAxis(); }
    int powerDataAxis() { return m_backend->powerDataAxis(); }
    int storageDepth() { return m_backend->storageDepth(); }

    SensorData accelerationData() const { return m_backend->accelerationData(); }
    SensorData gyroData() const { return m_backend->gyroData(); }
    SensorData magnetoData() const { return m_backend->magnetoData(); }
    SensorData powerData() const {return m_backend->powerData(); }

    void shutdown(){ m_backend->shutdown(); }
    void sleep() { m_backend->sleep(); }

private:
    const BackEnd *m_backend;
};



#endif // BACKEND_H

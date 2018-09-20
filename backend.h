#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QVector>
#include <QPointF>
#include <QPointer>
#include <QProcess>
#include <QRemoteObjectHost>

#include "rep_remotebackend_source.h"
#include "platformcontrol.h"
#include "sensordata.h"

class ReadOnlyRemoteBackend;

class BackEndInterface : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString serverAddress READ serverAddress NOTIFY serverAddressChanged)
    Q_PROPERTY(bool remotingEnabled READ remotingEnabled WRITE setRemotingEnabled NOTIFY remotingEnabledChanged)

public:
    BackEndInterface(QObject *parent = nullptr) : QObject(parent) {}
    ~BackEndInterface(){}

    virtual Q_INVOKABLE int accelerationDataAxis() const {return 0;}
    virtual Q_INVOKABLE int gyroDataAxis() const {return 0;}
    virtual Q_INVOKABLE int magnetoDataAxis() const {return 0;}
    virtual Q_INVOKABLE int powerDataAxis() const {return 0;}

    virtual Q_INVOKABLE int storageDepth() const {return 0;}

public Q_SLOTS:
    // Remote stuff
    virtual QString serverAddress() const {return "";}

    virtual bool remotingEnabled() const {return false;}
    virtual void setRemotingEnabled(bool enable) {(void)enable;}

    virtual void shutdown() const {}
    virtual void sleep() const {}
    virtual QString test() const {return "";}

Q_SIGNALS:
    void accelerationDataChanged(const SensorData &sensorData);
    void gyroDataChanged(const SensorData &sensorData);
    void magnetoDataChanged(const SensorData &sensorData);
    void powerDataChanged(const SensorData &sensorData);


    // Remote stuff
    void remotingEnabledChanged(bool remotingEnabled);
    void serverAddressChanged();
};

class BackEnd : public BackEndInterface
{
    Q_OBJECT
    Q_PROPERTY(QString serverAddress READ serverAddress NOTIFY serverAddressChanged)
    Q_PROPERTY(bool remotingEnabled READ remotingEnabled WRITE setRemotingEnabled NOTIFY remotingEnabledChanged)

public:
    explicit BackEnd(PlatformControl *controller = nullptr, QObject *parent = nullptr);

    void initializeData(QVector<QVector<QPointF> > &data, int rowCount, int colCount);

    Q_INVOKABLE int accelerationDataAxis() const;
    Q_INVOKABLE int gyroDataAxis() const;
    Q_INVOKABLE int magnetoDataAxis() const;
    Q_INVOKABLE int powerDataAxis() const;

    Q_INVOKABLE int storageDepth() const;

public Q_SLOTS:
    void processData(const QString &str);

    // Remote stuff
    QString serverAddress() const { return m_serverAddress; }

    bool remotingEnabled() const;
    void setRemotingEnabled(bool enable);

    void shutdown() const;
    void sleep() const;
    QString test() const;

Q_SIGNALS:
    void accelerationDataChanged(const SensorData &sensorData);
    void gyroDataChanged(const SensorData &sensorData);
    void magnetoDataChanged(const SensorData &sensorData);
    void powerDataChanged(const SensorData &sensorData);


    // Remote stuff
    void remotingEnabledChanged(bool remotingEnabled);
    void serverAddressChanged();


private:
    SensorData m_accelerationData;
    SensorData m_gyroData;
    SensorData m_magnetoData;
    SensorData m_powerData;

    PlatformControl &m_controller;

    int m_accelerationPos;
    int m_gyroPos;
    int m_magnetoPos;
    int m_powerPos;

    // Remote stuff
    void updateServerAddress();

    QString m_serverAddress;

    QPointer<QProcess> m_remoteAccess;
    QScopedPointer<QRemoteObjectHost> m_backendServer;
    QScopedPointer<ReadOnlyRemoteBackend> m_remoteBackend;
};

class ReadOnlyRemoteBackend : public RemoteBackendSource
{
    Q_OBJECT
public:
    explicit ReadOnlyRemoteBackend(const BackEnd *backend, QObject *parent = nullptr);

    QString serverAddress() const { return m_backend->serverAddress(); }
    bool remotingEnabled() const { return m_backend->remotingEnabled(); }
    void setRemotingEnabled(bool) { }

    int accelerationDataAxis() { return m_backend->accelerationDataAxis(); }
    int gyroDataAxis() { return m_backend->gyroDataAxis(); }
    int magnetoDataAxis() { return m_backend->magnetoDataAxis(); }
    int powerDataAxis() { return m_backend->powerDataAxis(); }
    int storageDepth() { return m_backend->storageDepth(); }
    void shutdown(){ m_backend->shutdown(); }
    void sleep() { m_backend->sleep(); }
    QString test() { return m_backend->test(); }

private:
    const BackEnd *m_backend;
};



#endif // BACKEND_H

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

class ReadOnlyRemoteBackend;

class BackEndInterface : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString serverAddress READ serverAddress NOTIFY serverAddressChanged)
    Q_PROPERTY(bool remotingEnabled READ remotingEnabled WRITE setRemotingEnabled NOTIFY remotingEnabledChanged)

public:
    BackEndInterface(QObject *parent = nullptr) : QObject(parent) {}
    ~BackEndInterface(){}

    virtual Q_INVOKABLE QPointF accelerationData(int sensor, int at) const {}
    virtual Q_INVOKABLE int accelerationDataAxis() const {}

    virtual Q_INVOKABLE QPointF gyroData(int sensor, int at) const {}
    virtual Q_INVOKABLE int gyroDataAxis() const {}

    virtual Q_INVOKABLE QPointF magnetoData(int sensor, int at) const {}
    virtual Q_INVOKABLE int magnetoDataAxis() const {}

    virtual Q_INVOKABLE QPointF powerData(int sensor, int at) const {}
    virtual Q_INVOKABLE int powerDataAxis() const {}

    virtual Q_INVOKABLE int storageDepth() const {}

public Q_SLOTS:
    // Remote stuff
    virtual QString serverAddress() const {}

    virtual bool remotingEnabled() const {}
    virtual void setRemotingEnabled(bool enable) {}

    virtual void shutdown() const {}
    virtual void sleep() const {}
    virtual QString test() const {}

Q_SIGNALS:
    void accelerationDataChanged();
    void gyroDataChanged();
    void magnetoDataChanged();
    void powerDataChanged();


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

    Q_INVOKABLE QPointF accelerationData(int sensor, int at) const;
    Q_INVOKABLE int accelerationDataAxis() const;

    Q_INVOKABLE QPointF gyroData(int sensor, int at) const;
    Q_INVOKABLE int gyroDataAxis() const;

    Q_INVOKABLE QPointF magnetoData(int sensor, int at) const;
    Q_INVOKABLE int magnetoDataAxis() const;

    Q_INVOKABLE QPointF powerData(int sensor, int at) const;
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
    void accelerationDataChanged();
    void gyroDataChanged();
    void magnetoDataChanged();
    void powerDataChanged();


    // Remote stuff
    void remotingEnabledChanged(bool remotingEnabled);
    void serverAddressChanged();


private:
    QVector<QVector<QPointF>> m_accelerationData;
    QVector<QVector<QPointF>> m_gyroData;
    QVector<QVector<QPointF>> m_magnetoData;
    QVector<QVector<QPointF>> m_powerData;

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

    QPointF accelerationData(int sensor, int at) { return m_backend->accelerationData(sensor, at); }
    int accelerationDataAxis() { return m_backend->accelerationDataAxis(); }
    QPointF gyroData(int sensor, int at) { return m_backend->gyroData(sensor, at); }
    int gyroDataAxis() { return m_backend->gyroDataAxis(); }
    QPointF magnetoData(int sensor, int at) { return m_backend->magnetoData(sensor, at); }
    int magnetoDataAxis() { return m_backend->magnetoDataAxis(); }
    QPointF powerData(int sensor, int at) { return m_backend->powerData(sensor, at); }
    int powerDataAxis() { return m_backend->powerDataAxis(); }
    int storageDepth() { return m_backend->storageDepth(); }
    void shutdown(){ m_backend->shutdown(); }
    void sleep() { m_backend->sleep(); }
    QString test() { return m_backend->test(); }

private:
    const BackEnd *m_backend;
};



#endif // BACKEND_H

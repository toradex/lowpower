#ifndef REMOTEBACKENDWRAPPER_H
#define REMOTEBACKENDWRAPPER_H

#include <QObject>

#include "rep_remotebackend_replica.h"
#include "backend.h"

class RemoteBackEndWrapper : public BackEndInterface
{
    Q_OBJECT
    Q_PROPERTY(QString serverAddress READ serverAddress NOTIFY serverAddressChanged)
    Q_PROPERTY(bool remotingEnabled READ remotingEnabled WRITE setRemotingEnabled NOTIFY remotingEnabledChanged)

public:
    RemoteBackEndWrapper(RemoteBackendReplica *backend, QObject *parent = nullptr);
    ~RemoteBackEndWrapper() {}

    QString serverAddress() const { return m_backend->serverAddress(); }
    bool remotingEnabled() const { return m_backend->remotingEnabled(); }
    void setRemotingEnabled(bool) {}

    Q_INVOKABLE QPointF accelerationData(int sensor, int at) const {
        QRemoteObjectPendingReply<QPointF> val = m_backend->accelerationData(sensor, at);
        val.waitForFinished();
        return val.returnValue();
    }
    Q_INVOKABLE int accelerationDataAxis() const {
        QRemoteObjectPendingReply<int> val = m_backend->accelerationDataAxis();
        val.waitForFinished();
        return val.returnValue();
    }
    Q_INVOKABLE QPointF gyroData(int sensor, int at) const {
        QRemoteObjectPendingReply<QPointF> val = m_backend->gyroData(sensor, at);
        val.waitForFinished();
        return val.returnValue();
    }
    Q_INVOKABLE int gyroDataAxis() const {
        QRemoteObjectPendingReply<int> val = m_backend->gyroDataAxis();
        val.waitForFinished();
        return val.returnValue();
    }
    Q_INVOKABLE QPointF magnetoData(int sensor, int at) const {
        QRemoteObjectPendingReply<QPointF> val = m_backend->magnetoData(sensor, at);
        val.waitForFinished();
        return val.returnValue();
    }
    Q_INVOKABLE int magnetoDataAxis() const {
        QRemoteObjectPendingReply<int> val = m_backend->magnetoDataAxis();
        val.waitForFinished();
        return val.returnValue();
    }
    Q_INVOKABLE QPointF powerData(int sensor, int at) const {
        QRemoteObjectPendingReply<QPointF> val = m_backend->powerData(sensor, at);
        val.waitForFinished();
        return val.returnValue();
    }
    Q_INVOKABLE int powerDataAxis() const {
        QRemoteObjectPendingReply<int> val = m_backend->powerDataAxis();
        val.waitForFinished();
        return val.returnValue();
    }
    Q_INVOKABLE int storageDepth() const {
        QRemoteObjectPendingReply<int> val = m_backend->storageDepth();
        val.waitForFinished();
        return val.returnValue();
    }
    Q_INVOKABLE void shutdown() const { m_backend->shutdown(); }
    Q_INVOKABLE void sleep() const { m_backend->sleep(); }
    Q_INVOKABLE QString test() const {
        QRemoteObjectPendingReply<QString> val = m_backend->test();
        val.waitForFinished();
        return val.returnValue();
    }

Q_SIGNALS:
    void accelerationDataChanged();
    void gyroDataChanged();
    void magnetoDataChanged();
    void powerDataChanged();
    // Remote stuff
    void remotingEnabledChanged(bool remotingEnabled);
    void serverAddressChanged();

private:
    RemoteBackendReplica *m_backend;
};

#endif // REMOTEBACKENDWRAPPER_H

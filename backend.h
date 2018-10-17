#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QVector>
#include <QPointF>
#include <QPointer>
#include <QProcess>
#include <QRemoteObjectHost>
#include <QAbstractSeries>
#include <QTimer>


#include "rep_remotebackend_source.h"
#include "platformcontrol.h"
#include "sensordata.h"

class ReadOnlyRemoteBackend;

QT_CHARTS_USE_NAMESPACE

class BackEnd : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString serverAddress READ serverAddress NOTIFY serverAddressChanged)
    Q_PROPERTY(bool remotingEnabled READ remotingEnabled WRITE setRemotingEnabled NOTIFY remotingEnabledChanged)

    Q_PROPERTY(SensorData accelerationData READ accelerationData NOTIFY accelerationDataChanged)
    Q_PROPERTY(SensorData gyroData READ gyroData NOTIFY gyroDataChanged)
    Q_PROPERTY(SensorData magnetoData READ magnetoData NOTIFY magnetoDataChanged)
    Q_PROPERTY(SensorData powerData READ powerData NOTIFY powerDataChanged)

    Q_PROPERTY(int tabIndex READ tabIndex WRITE setTabIndex NOTIFY tabIndexChanged)

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

    int tabIndex() const;

public Q_SLOTS:
    void processData(const QByteArray &data);

    // Remote stuff
    QString serverAddress() const { return m_serverAddress; }

    bool remotingEnabled() const;
    void setRemotingEnabled(bool enable);

    void shutdown() const;
    void sleep() const;
    QString test() const;
    void setTabIndex(int tabIndex);

Q_SIGNALS:
    // Remote stuff
    void remotingEnabledChanged(bool remotingEnabled);
    void serverAddressChanged();

    void accelerationDataChanged(SensorData sensorData);
    void gyroDataChanged(SensorData sensorData);
    void magnetoDataChanged(SensorData sensorData);
    void powerDataChanged(SensorData sensorData);

    void tabIndexChanged(int tabIndex);

private:
    SensorData m_accelerationData;
    SensorData m_gyroData;
    SensorData m_magnetoData;
    SensorData m_powerData;

    int m_tabIndex;

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
    explicit ReadOnlyRemoteBackend(BackEnd *backend, QObject *parent = nullptr);

    QString serverAddress() const { return m_backend->serverAddress(); }
    bool remotingEnabled() const { return m_backend->remotingEnabled(); }
    // We can't disable the remote connection directly because it would crash, therefore we delay it so
    // that we are sure the event is triggered in the right context
    void setRemotingEnabled(bool enable) {  if (!enable) QTimer::singleShot(200, this, SLOT(disableRemoting())); }

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

    void setTabIndex(int tabIndex) {m_backend->setTabIndex(tabIndex);}
    int tabIndex() const { return m_backend->tabIndex(); }
private slots:
    void disableRemoting(){m_backend->setRemotingEnabled(false);}
private:
    BackEnd *m_backend;
};



#endif // BACKEND_H

#include "remotebackendwrapper.h"

RemoteBackEndWrapper::RemoteBackEndWrapper(RemoteBackendReplica *backend, QObject *parent) :
    BackEndInterface(parent), m_backend(backend)
{
    connect(m_backend, &RemoteBackendReplica::accelerationDataChanged, this, &RemoteBackEndWrapper::accelerationDataChanged, Qt::QueuedConnection);
//    connect(m_backend, &RemoteBackendReplica::gyroDataChanged, this, &RemoteBackEndWrapper::gyroDataChanged, Qt::QueuedConnection);
//    connect(m_backend, &RemoteBackendReplica::magnetoDataChanged, this, &RemoteBackEndWrapper::magnetoDataChanged, Qt::QueuedConnection);
//    connect(m_backend, &RemoteBackendReplica::powerDataChanged, this, &RemoteBackEndWrapper::powerDataChanged, Qt::QueuedConnection);

//    connect(m_backend, &RemoteBackendReplica::remotingEnabled, this, &RemoteBackEndWrapper::remotingEnabled, Qt::QueuedConnection);
//    connect(m_backend, &RemoteBackendReplica::serverAddressChanged, this, &RemoteBackEndWrapper::serverAddressChanged, Qt::QueuedConnection);
}

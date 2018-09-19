#ifndef REMOTEEVENTHANDLER_H
#define REMOTEEVENTHANDLER_H

#include <QObject>
#include <qpointer.h>

#include "rep_remoteeventhandler_source.h"
#include "rep_remoteeventhandler_replica.h"

Q_DECLARE_LOGGING_CATEGORY(remoteEventHandler)

class RemoteEventHandler : public RemoteEventHandlerSource
{
    Q_OBJECT
public:
    explicit RemoteEventHandler(QObject *parent = nullptr);
    ~RemoteEventHandler();

    Q_INVOKABLE void startHandlingEvents(QObject *watched);
    Q_INVOKABLE void stopHandlingEvents();

Q_SIGNALS:
    void remoteMouseXChanged(int x);
    void remoteMouseYChanged(int y);
    void remoteMouseClicked();

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private Q_SLOTS:
    void handleRemoteMouseEvent(int type, const QPointF &localPos, int buttons, int modifiers);
    void handleRemoteKeyEvent(int type, int key, int modifiers, const QString &text, bool autorep, ushort count);

private:
    QPointer<QObject> m_watchedObject;
    QScopedPointer<QRemoteObjectHost> m_eventSender;
    QScopedPointer<QRemoteObjectNode> m_eventReceiver;
    QScopedPointer<RemoteEventHandlerReplica> m_remoteEventHandler;
};

#endif // REMOTEEVENTHANDLER_H

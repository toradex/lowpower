#include "remoteeventhandler.h"

#include <QGuiApplication>
#include <qevent.h>
#include <qpa/qwindowsysteminterface.h>

#define HOST_URL "local:hosteventhandler"
#define REMOTE_URL "local:remoteeventhandler"

Q_LOGGING_CATEGORY(remoteEventHandler, "remoteEventHandler")

RemoteEventHandler::RemoteEventHandler(QObject *parent)
    : RemoteEventHandlerSource(parent)
{
}

RemoteEventHandler::~RemoteEventHandler()
{
    stopHandlingEvents();
}

void RemoteEventHandler::startHandlingEvents(QObject *watched)
{
    if (m_watchedObject == watched)
        return;

    stopHandlingEvents();

    m_watchedObject = watched;

    const bool isHost = qApp->platformName() != QLatin1String("webgl");

    m_eventReceiver.reset(new QRemoteObjectNode);
    m_eventReceiver->connectToNode(QUrl(isHost ? REMOTE_URL : HOST_URL));
    m_remoteEventHandler.reset(m_eventReceiver->acquire<RemoteEventHandlerReplica>());
    connect(m_remoteEventHandler.data(), &RemoteEventHandlerReplica::mouseEvent, this, &RemoteEventHandler::handleRemoteMouseEvent);
    connect(m_remoteEventHandler.data(), &RemoteEventHandlerReplica::keyEvent, this, &RemoteEventHandler::handleRemoteKeyEvent);

    m_eventSender.reset(new QRemoteObjectHost);
    m_eventSender->setHostUrl(QUrl(isHost ? HOST_URL : REMOTE_URL));
    m_eventSender->enableRemoting(this);

    qApp->installEventFilter(this);
}

void RemoteEventHandler::stopHandlingEvents()
{
    if (m_watchedObject) {
        qApp->removeEventFilter(this);
        m_watchedObject.clear();

        m_remoteEventHandler.reset();
        m_eventSender.reset();
        m_eventReceiver.reset();
    }
}

bool RemoteEventHandler::eventFilter(QObject *watched, QEvent *event)
{
    if (event->spontaneous()) {
        switch (event->type()) {
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonRelease:
        case QEvent::MouseButtonDblClick:
        case QEvent::MouseMove: {
            QMouseEvent *me = static_cast<QMouseEvent *>(event);

            // don't resend the remote events we just received
            if (me->source() == Qt::MouseEventSynthesizedByApplication)
                break;

            QVariant point;
            QMetaObject::invokeMethod(m_watchedObject.data(), "mapFromWindow",
                                      Q_RETURN_ARG(QVariant, point),
                                      Q_ARG(QVariant, me->localPos().x()),
                                      Q_ARG(QVariant, me->localPos().y()));

            emit this->mouseEvent(event->type(), point.toPoint(), me->buttons(), me->modifiers());
            break;
        }
        case QEvent::KeyPress:
        case QEvent::KeyRelease: {
            QKeyEvent *ke = static_cast<QKeyEvent *>(event);
            // don't resend the remote events we just received
            if (ke->timestamp() == 0)
                break;
            emit this->keyEvent(event->type(), ke->key(), ke->modifiers(), ke->text(), ke->isAutoRepeat(), ke->count());
            break;
        }
        default:
            break;
        }
    }

    return QObject::eventFilter(watched, event);
}

void RemoteEventHandler::handleRemoteMouseEvent(int type, const QPointF &localPos, int buttons, int modifiers)
{
    QVariant point;
    QMetaObject::invokeMethod(m_watchedObject.data(), "mapToWindow",
                              Q_RETURN_ARG(QVariant, point),
                              Q_ARG(QVariant, localPos.x()),
                              Q_ARG(QVariant, localPos.y()));
    QPointF pos = point.toPointF();

    QWindow *w = qApp->allWindows().first();
    QWindowSystemInterface::handleMouseEvent(w, pos, w->mapToGlobal(pos.toPoint()),
                                             Qt::MouseButtons(buttons), Qt::KeyboardModifiers(modifiers),
                                             Qt::MouseEventSynthesizedByApplication);

    emit remoteMouseXChanged(localPos.x());
    emit remoteMouseYChanged(localPos.y());
    if (type == QEvent::MouseButtonRelease || type == QEvent::MouseButtonDblClick)
        emit remoteMouseClicked();
}

void RemoteEventHandler::handleRemoteKeyEvent(int type, int key, int modifiers, const QString &text, bool autorep, ushort count)
{
    QWindow *w = qApp->allWindows().first();
    // No way to mark key event as synthesized by application, set the timestamp to 0 instead
    QWindowSystemInterface::handleKeyEvent(w, 0, QEvent::Type(type), key, Qt::KeyboardModifiers(modifiers),
                                           text, autorep, count);
}

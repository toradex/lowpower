#ifndef PLATFORMCONTROL_H
#define PLATFORMCONTROL_H

#include <QObject>

class PlatformControl : public QObject
{
    Q_OBJECT
public:
    explicit PlatformControl(QObject *parent = nullptr);

signals:
    void dataReceived(const QString &data);

public slots:
    virtual void shutdown() = 0;
    virtual void sleep() = 0;
};

#endif // PLATFORMCONTROL_H

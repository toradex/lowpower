#ifndef PLATFORMCONTROLMOCK_H
#define PLATFORMCONTROLMOCK_H

#include <QObject>
#include <QTimer>

#include "platformcontrol.h"

class PlatformControlMock : public PlatformControl
{
    Q_OBJECT

public:
    PlatformControlMock(QObject *parent = nullptr);

private:
    QTimer timer;
    // PlatformControl interface
public slots:
    void shutdown();
    void sleep();
    void generateData();

};

#endif // PLATFORMCONTROLMOCK_H

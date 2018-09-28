#ifndef PLATFORMCONTROLRPMSG_H
#define PLATFORMCONTROLRPMSG_H

#include <QObject>
#include <QByteArray>
#include <QString>
#include <QtSerialPort/QSerialPort>

#include "platformcontrol.h"

class PlatformControlRpmsg : public PlatformControl
{
    Q_OBJECT

public:
    PlatformControlRpmsg(QObject *parent = nullptr);
    bool initialize();
    bool sendCommand(QByteArray data);

    // PlatformControl interface
public slots:
    void shutdown();
    void sleep();
    void handleReadyRead();

private:
    QSerialPort m_serialPort;

};
#endif // PLATFORMCONTROLRPMSG_H

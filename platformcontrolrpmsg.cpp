#include <QProcess>
#include <QFile>
#include <QDebug>
#include <QThread>
#include "platformcontrolrpmsg.h"

#define SERIAL_PORT "/dev/ttyRPMSG"

PlatformControlRpmsg::PlatformControlRpmsg(QObject *parent) : PlatformControl(parent)
{
    m_serialPort.setPortName(SERIAL_PORT);
    connect(&m_serialPort, &QSerialPort::readyRead, this, &PlatformControlRpmsg::handleReadyRead);
}

bool PlatformControlRpmsg::initialize()
{
    if (!m_serialPort.open(QIODevice::ReadWrite)) {
        return false;
    }
    return true;
}

void PlatformControlRpmsg::handleReadyRead()
{
#define PROTO_SIZE 24
    while (m_serialPort.bytesAvailable() > 0) {
        QByteArray data = m_serialPort.read(PROTO_SIZE);
        if (0xFF != data[0]) {
            qWarning() << "Out of sync, try to recover";
            return;
        }
        if (data.size() < PROTO_SIZE) {
            QThread::msleep(10);
            data.append(m_serialPort.read(PROTO_SIZE - data.size()));
            if (data.size() < PROTO_SIZE) {
                qWarning() << "Out of sync, try to recover";
                return;
            }
        }

        emit dataReceived(data);
    }
}


bool PlatformControlRpmsg::sendCommand(QByteArray data)
{
    if (m_serialPort.write(data, data.length()) < 0)
        return false;
    return true;
}

void PlatformControlRpmsg::shutdown()
{
    QString programName("poweroff");
    QProcess::execute(programName);
}

void PlatformControlRpmsg::sleep()
{
    QString programName("systemctl suspend");
    QProcess::execute(programName);
}

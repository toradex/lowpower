#include <QDebug>
#include <QRandomGenerator>

#include "platformcontrolmock.h"

PlatformControlMock::PlatformControlMock(QObject *parent) : PlatformControl (parent)
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(generateData()));
    timer.setInterval(1000);
    timer.start();
}

void PlatformControlMock::shutdown()
{
    qDebug() << "Shutdown";
}

void PlatformControlMock::sleep()
{
    qDebug() << "Sleep";
}

void PlatformControlMock::generateData()
{
    QRandomGenerator *generator = QRandomGenerator::global();
    static int i;
    QString str1 = QString::number(generator->generateDouble()*8-4);
    QString str2 = QString::number(generator->generateDouble()*8-4);
    QString str3 = QString::number(generator->generateDouble()*8-4);
    emit dataReceived("acc," + str1 + "," + str2 + "," + str3);

    str1 = QString::number(generator->generateDouble()*8-4);
    str2 = QString::number(generator->generateDouble()*8-4);
    str3 = QString::number(generator->generateDouble()*8-4);
    emit dataReceived("gyro," + str1 + "," + str2 + "," + str3);

    str1 = QString::number(generator->generateDouble()*8-4);
    str2 = QString::number(generator->generateDouble()*8-4);
    str3 = QString::number(generator->generateDouble()*8-4);
    emit dataReceived("mag," + str1 + "," + str2 + "," + str3);

    str1 = QString::number(generator->generateDouble()*600.0);
    emit dataReceived("alti," + str1);
    i++;
}

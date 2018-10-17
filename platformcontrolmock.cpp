#include <QDebug>
#include <QRandomGenerator>
#include <QtMath>

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
#define GYRO_DIVIDER 8
#define ACC_DIVIDER (8192.0/9.81)
#define MAG_DIVIDER 10
#define POWER_DIVIDER 8
#define CURRENT_DIVIDER 10000
    QRandomGenerator *generator = QRandomGenerator::global();
    static int i;
    QByteArray data;
    qreal number;

    data.append(0xFF);

    number = (generator->generateDouble()*8-4)*GYRO_DIVIDER;
    number = (number < 0) ? number + 0x10000 : number;
    data.append((qRound(number) >> 0) & 0xFF);
    data.append((qRound(number) >> 8) & 0xFF);
    number = (generator->generateDouble()*8-4)*GYRO_DIVIDER;
    number = (number < 0) ? number + 0x10000:number;
    data.append((qRound(number) >> 0) & 0xFF);
    data.append((qRound(number) >> 8) & 0xFF);
    number = (generator->generateDouble()*8-4)*GYRO_DIVIDER;
    number = (number < 0) ? number + 0x10000:number;
    data.append((qRound(number) >> 0) & 0xFF);
    data.append((qRound(number) >> 8) & 0xFF);


    number = (generator->generateDouble()*80-40)*ACC_DIVIDER;
    number = (number < 0) ? number + 0x10000:number;
    data.append((qRound(number) >> 0) & 0xFF);
    data.append((qRound(number) >> 8) & 0xFF);
    number = (generator->generateDouble()*80-40)*ACC_DIVIDER;
    number = (number < 0) ? number + 0x10000:number;
    data.append((qRound(number) >> 0) & 0xFF);
    data.append((qRound(number) >> 8) & 0xFF);
    number = (generator->generateDouble()*80-40)*ACC_DIVIDER;
    number = (number < 0) ? number + 0x10000:number;
    data.append((qRound(number) >> 0) & 0xFF);
    data.append((qRound(number) >> 8) & 0xFF);


    number = (generator->generateDouble()*100-50)*MAG_DIVIDER;
    number = (number < 0) ? number + 0x10000:number;
    data.append((qRound(number) >> 0) & 0xFF);
    data.append((qRound(number) >> 8) & 0xFF);
    number = (generator->generateDouble()*100-50)*MAG_DIVIDER;
    number = (number < 0) ? number + 0x10000:number;
    data.append((qRound(number) >> 0) & 0xFF);
    data.append((qRound(number) >> 8) & 0xFF);
    number = (generator->generateDouble()*100-50)*MAG_DIVIDER;
    number = (number < 0) ? number + 0x10000:number;
    data.append((qRound(number) >> 0) & 0xFF);
    data.append((qRound(number) >> 8) & 0xFF);

    /* Charge stat */
    data.append(0.0);

    number = (generator->generateDouble()*600)*POWER_DIVIDER;
    data.append((qRound(number) >> 0) & 0xFF);
    data.append((qRound(number) >> 8) & 0xFF);

    number = (generator->generateDouble()*8)*CURRENT_DIVIDER;
    data.append((qRound(number) >> 0) & 0xFF);
    data.append((qRound(number) >> 8) & 0xFF);

    emit dataReceived(data);
}

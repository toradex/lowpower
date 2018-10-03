#ifndef SENSORDATA_H
#define SENSORDATA_H

#include <QObject>
#include <QVector>
#include <QPointF>
#include <QVariant>
#include <QDataStream>

class SensorData
{
    // Use Q_GADGET for registering a Type (not inherited from QObject)
    // We shouldn't inherit from QObject because we need a copy constructor
    Q_GADGET

    // Properties that are accessible from QML
    Q_PROPERTY(int axis MEMBER m_axis)
    Q_PROPERTY(int points READ points)

public:
    explicit SensorData(int axis = 0, int points = 0);
    SensorData(const SensorData &other);
    ~SensorData();

    void push_pop(int axis, qreal point) {
        m_data[axis].pop_back();
        m_data[axis].push_front(point);
    }

    void push_front(int axis, qreal point) {
        m_data[axis].push_front(point);
    }

    void pop_back(int axis) {
        m_data[axis].pop_back();
    }

    // At is used from QML to get the different points
    Q_INVOKABLE qreal at(int axis, int point) const {
        return m_data[axis][point];
    }

    // We need to serialize this object, so that we can transmit it to the remote Application
    friend QDataStream &operator <<(QDataStream& stream, const SensorData& data) {
        stream << data.m_axis;
        stream << data.m_points;
        for (int i = 0; i < data.m_axis; i++) {
            stream << data.m_data[i];
        }
        return stream;
    }
    friend QDataStream &operator >>(QDataStream& stream, SensorData& data){
        //QVariant var;
        stream >> data.m_axis;
        //data.m_axis = var.toInt();
        stream >> data.m_points;
        //data.m_points = var.toInt();

        data.m_data.clear();

        for (int i = 0; i < data.m_axis; i++) {
            QVector<qreal> points;
            stream >> points;
            data.m_data.append(points);
        }

        return stream;
    }

    inline bool operator!=(const SensorData& cmp){ return !(&this->m_data == &cmp.m_data); }
    int points() const;

private:
    int m_axis;
    int m_points;

    QVector<QVector<qreal>> m_data;
};

Q_DECLARE_METATYPE(SensorData)

#endif // SENSORDATA_H

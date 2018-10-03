#include "sensordata.h"

SensorData::SensorData(int axis, int points): m_axis(axis), m_points(points), m_data(axis)
{
    for (int i = 0; i < axis; i++) {
        for (int j = 0; j < points; j++) {
            m_data[i].append(0.0);
        }
    }
}


SensorData::SensorData(const SensorData &other)
{
    m_data = other.m_data;
    m_axis = other.m_axis;
    m_points = other.m_points;
}

SensorData::~SensorData()
{

}

int SensorData::points() const
{
    return m_points;
}

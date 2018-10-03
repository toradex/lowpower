#include "processsettings.h"

ProcessSettings::ProcessSettings(bool webgl, QObject *parent) : QObject(parent), m_webgl(webgl)
{

}

bool ProcessSettings::webgl() const
{
    return m_webgl;
}

void ProcessSettings::setWebgl(bool webgl)
{
    m_webgl = webgl;
    emit webglChanged(webgl);
}

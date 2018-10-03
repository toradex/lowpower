#ifndef PROCESSSETTINGS_H
#define PROCESSSETTINGS_H

#include <QObject>

class ProcessSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool webgl READ webgl WRITE setWebgl NOTIFY webglChanged)
public:
    explicit ProcessSettings(bool webgl = false, QObject *parent = nullptr);

    bool webgl() const;
    void setWebgl(bool webgl);

signals:
    void webglChanged(bool webgl);

public slots:

private:
    bool m_webgl;
};

#endif // PROCESSSETTINGS_H

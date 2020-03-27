/**
 *  OSM
 *  Copyright (C) 2018  Pavel Smokotnin

 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef GENERATOR_H
#define GENERATOR_H

#include <QObject>
#include <QDebug>

#include "generatorthread.h"
#include "settings.h"
#include "sinsweep.h"

class Generator : public QObject
{
    Q_OBJECT

    //On-off property
    Q_PROPERTY(bool enabled READ getEnabled WRITE setEnabled NOTIFY enabledChanged)

    //current type
    Q_PROPERTY(int type READ getType WRITE setType NOTIFY typeChanged)

    //Available generator types. Constant list.
    Q_PROPERTY(QVariant types READ getAvailableTypes CONSTANT)

    //Available output devices
    Q_PROPERTY(QVariant devices READ getDeviceList CONSTANT)
    Q_PROPERTY(QString device READ deviceName WRITE selectDevice NOTIFY deviceChanged)

    //Frequency
    Q_PROPERTY(int frequency READ frequency WRITE setFrequency NOTIFY frequencyChanged)
    Q_PROPERTY(int startFrequency READ startFrequency WRITE setStartFrequency NOTIFY startFrequencyChanged)
    Q_PROPERTY(int endFrequency READ endFrequency WRITE setEndFrequency NOTIFY endFrequencyChanged)
    Q_PROPERTY(SinSweep::Type sweepType READ sweepType WRITE setSweepType NOTIFY sweepTypeChanged)

    //Duration
    Q_PROPERTY(int duration READ duration WRITE setDuration NOTIFY durationChanged)

    //Gain
    Q_PROPERTY(float gain READ gain WRITE setGain NOTIFY gainChanged)

    Q_PROPERTY(int channelsCount READ channelsCount NOTIFY channelsCountChanged)
    Q_PROPERTY(int channel READ channel WRITE setChannel NOTIFY channelChanged)
    Q_PROPERTY(int aux READ aux WRITE setAux NOTIFY auxChanged)

private:
    GeneratorThread m_thread;
    Settings *m_settings;

    void loadSettings();

public:
    explicit Generator(Settings *settings, QObject* parent = nullptr);
    ~Generator();

    bool getEnabled() const {return m_thread.enabled();}
    void setEnabled(bool enable);

    int getType() const {return m_thread.type();}
    void setType(int t);

    QVariant getAvailableTypes(void) const {return m_thread.getAvailableTypes();}
    QVariant getDeviceList(void) const {return m_thread.getDeviceList();}

    QString deviceName() const {return m_thread.deviceName();}
    void selectDevice(const QString &name);

    int frequency() const {return m_thread.frequency();}
    void setFrequency(int f);

    int startFrequency() const {return m_thread.startFrequency();}
    void setStartFrequency(int f);

    int endFrequency() const {return m_thread.endFrequency();}
    void setEndFrequency(int f);

    float gain() const {return m_thread.gain();}
    void setGain(float gain);

    int channelsCount() const {return m_thread.channelsCount();}
    int channel() const {return m_thread.channel();}
    void setChannel(int channel);
    int aux() const {return m_thread.aux();}
    void setAux(int channel);

    SinSweep::Type sweepType() const {return m_thread.sweepType();}

    int duration() const{return m_thread.duration(); }

signals:
    void enabledChanged(bool);
    void typeChanged();
    void frequencyChanged(int f);
    void startFrequencyChanged(int f);
    void endFrequencyChanged(int f);
    void deviceChanged();
    void gainChanged(float);
    void channelsCountChanged();
    void channelChanged(int);
    void auxChanged(int);
    void sweepTypeChanged(int sweepType);
    void durationChanged(int duration);

public slots:
void setSweepType(int sweepType);
void setDuration(int duration);
};

#endif // GENERATOR_H

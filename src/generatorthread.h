/**
 *  OSM
 *  Copyright (C) 2019  Pavel Smokotnin

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
#ifndef GENERATORTHREAD_H
#define GENERATORTHREAD_H

#include <QThread>
#include <QAudioOutput>

#include "outputdevice.h"
#include "sinsweep.h"

class GeneratorThread : public QThread
{
    Q_OBJECT

public:
    explicit GeneratorThread(QObject *parent);

private:
    QAudioOutput* m_audio;
    QAudioFormat m_format;
    QAudioDeviceInfo m_device;
    QList<OutputDevice*> m_sources;

    float m_gain;
    int m_type;
    int m_frequency, m_startFrequency, m_endFrequency, m_duration;
    int m_channelCount, m_channel, m_aux;
    bool m_enabled;
    SinSweep::Type m_sweepType;

    void _selectDevice(const QAudioDeviceInfo &device);
    void _updateAudio();

public slots:
    void init();
    void finish();

    bool enabled() const {return m_enabled;}
    void setEnabled(bool enable);

    int type() const {return m_type;}
    void setType(int type);

    QVariant getDeviceList() const;
    void selectDevice(const QString &name);

    QString deviceName() const;
    QVariant getAvailableTypes() const;

    int frequency() const {return m_frequency;}
    void setFrequency(int frequency);

    int startFrequency() const {return m_startFrequency;}
    void setStartFrequency(int startFrequency);

    int endFrequency() const {return m_endFrequency;}
    void setEndFrequency(int endFrequency);

    int duration() const {return m_duration;}
    void setDuration(int duration);

    float gain() const {return m_gain;}
    void setGain(float gain);

    SinSweep::Type sweepType() const {return m_sweepType;}
    void setSweepType(const SinSweep::Type sweepType);

    int channelsCount() const {return m_channelCount;}
    int channel() const {return m_channel;}
    void setChannel(int channel);

    int aux() const {return m_aux;}
    void setAux(int channel);

signals:
    void enabledChanged(bool);
    void deviceChanged(QString);
    void typeChanged(int);
    void frequencyChanged(int);
    void startFrequencyChanged(int);
    void endFrequencyChanged(int);
    void gainChanged(float);
    void durationChanged(int);
    void channelChanged(int);
    void auxChanged(int);
    void channelsCountChanged();
    void sweepTypeChanged(int);
};

#endif // GENERATORTHREAD_H

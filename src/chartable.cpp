#include "chartable.h"

Chartable::Chartable(QObject *parent) : QIODevice(parent)
{

}
void Chartable::alloc()
{
    data = (complex *)calloc(_fftSize, sizeof(complex));
    referenceData = (complex *)calloc(_fftSize, sizeof(complex));
    impulseData = (complex *)calloc(_fftSize, sizeof(complex));

    dataStack = new AudioStack(_fftSize);
    referenceStack = new AudioStack(_fftSize);
}

qint64 Chartable::readData(char *data, qint64 maxlen)
{
    Q_UNUSED(data);
    Q_UNUSED(maxlen);

    return -1;
}
qint64 Chartable::writeData(const char *data, qint64 len)
{
    Q_UNUSED(data);
    Q_UNUSED(len);

    return -1;
}
void Chartable::setActive(bool active)
{
    _active = active;
    emit activeChanged();
}

void Chartable::updateSeries(QAbstractSeries *series, QString type)
{
    if (!series)
        return;

    //clean
    if (type == "") {
        QXYSeries *xySeries = static_cast<QXYSeries *>(series);
        xySeries->clear();
        return;
    }

    if (type == "Scope")
        return scopeSeries(series);
    if (type == "Impulse")
        return impulseSeries(series);

        QXYSeries *xySeries = static_cast<QXYSeries *>(series);

        QVector<QPointF> points;

        int i = 0;
        float startFrequency     = 6.875, //(note A)
                frequencyFactor  = pow(2, 1.0 / _pointsPerOctave),
                currentFrequency = startFrequency,
                nextFrequency    = currentFrequency * frequencyFactor,
                currentLevel     = 0.0,
                rateFactor       = sampleRate() / _fftSize,
                m, y, f, p
                ;
        int     currentCount     = 0;

        for (i = 0; i < _fftSize / 2; i ++) {
            m = std::abs(data[i]);
            p = std::arg(data[i]) - std::arg(referenceData[i]);
            p *= 180.0 / M_PI;

            //if (type == "RTA") m /= 1.0; - 1.0f - 0dB
            if (type == "Magnitude")
                m /= std::abs(referenceData[i]);


            y = (type == "Phase" ? p : 20.0 * log10(m));
            f = i * rateFactor;
            currentCount ++;

            if (_pointsPerOctave >= 1) {

                if (f > currentFrequency + (nextFrequency - currentFrequency) / 2) {

                    y = (type == "Phase" ?
                             currentLevel / currentCount :
                             20.0 * log10(currentLevel / currentCount)
                             );

                    points.append(QPointF(currentFrequency, y));
                    currentLevel     = 0.0;
                    currentCount     = 0;

                    while (f > currentFrequency + (nextFrequency - currentFrequency) / 2) {
                        currentFrequency = nextFrequency;
                        nextFrequency    = currentFrequency * frequencyFactor;
                    }
                }

                currentLevel += (type == "Phase" ? p : m);
            } else {
                //without grouping by freq data
                if (f == 0)
                    f = std::numeric_limits<float>::min();
                points.append(QPointF(f, y));
            }
        }

        xySeries->replace(points);

}
void Chartable::scopeSeries(QAbstractSeries *series)
{
    QXYSeries *xySeries = static_cast<QXYSeries *>(series);

    QVector<QPointF> points;
    float trigLevel = 0.0, lastLevel = NULL;
    float x, y;
    int i, trigPoint = _fftSize / 2;
    dataStack->reset();

    for (i = 0; i < 3 * _fftSize / 4; i++, dataStack->next()) {

        if (i < _fftSize / 4)
            continue;

        if (lastLevel != NULL)
        {
            if (lastLevel <= trigLevel && dataStack->current() >= trigLevel) {
                trigPoint = i;
                break;
            }
        }
        lastLevel = dataStack->current();
    }

    dataStack->reset();
    i = 0;
    do {
        x = (i - trigPoint) / 48.0;
        y = dataStack->current();
        points.append(QPointF(x, y));
        i++;
    }
    while (dataStack->next());
    xySeries->replace(points);
}
void Chartable::impulseSeries(QAbstractSeries *series)
{
    QXYSeries *xySeries = static_cast<QXYSeries *>(series);

    QVector<QPointF> points;
    float x, y;

    for (int i = 0; i < _fftSize; i ++) {
        x = i / 48.0;
        y = impulseData[i].real();
        points.append(QPointF(x, y));
    }

    xySeries->replace(points);
}
void Chartable::copyData(AudioStack *toDataStack,
              AudioStack *toReferenceStack,
              complex *toData,
              complex *toReferenceData,
              complex *toImpulseData)
{
    dataStack->reset();
    referenceStack->reset();
    for (int i = 0; i < fftSize(); i++) {
        toDataStack->add(dataStack->current());
        toReferenceStack->add(referenceStack->current());

        toData[i] = data[i];
        toReferenceData[i] = referenceData[i];
        toImpulseData[i] = impulseData[i];

        dataStack->next();
        referenceStack->next();
    }
}
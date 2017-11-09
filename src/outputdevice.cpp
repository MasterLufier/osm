#include "outputdevice.h"

OutputDevice::OutputDevice(QObject *parent) : QIODevice(parent)
{

}

/**
 * @brief OutputDevice::readData makes silent output stream by default.
 * @param data
 * @param maxlen
 * @return
 */
/*qint64 OutputDevice::readData(char *data, qint64 maxlen)
{
    for (qint64 i = 0; i < maxlen; i++) {
        data[i] = 0;
    }
    return maxlen;
}*/
qint64 OutputDevice::readData(char *data, qint64 maxlen)
{
    qint64 total = 0;
    while (maxlen - total > 0) {
        const Sample src = this->sample();
        memcpy(data + total, &src.f, sizeof(float));
        total += sizeof(float);
    }
    return total;
}
Sample OutputDevice::sample(void)
{
    Sample output;
    output.f = 0.0;
    return output;
}
qint64 OutputDevice::writeData(const char *data, qint64 len)
{
    Q_UNUSED(data);
    Q_UNUSED(len);

    return -1;
}

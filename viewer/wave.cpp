/**************************************************************************
 * Copyright (c) 2015 Afa.L Cheng <afa@afa.moe>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 ***************************************************************************/


#include "wave.h"

#include <QFile>
#include <ipp.h>

Wave::Wave()
{

}

Wave::~Wave()
{
    for (auto ptr : _data)
        ippFree(ptr);
}


Wave Wave::read(const QString &path)
{
    QFile f(path);
    f.open(QIODevice::ReadOnly);

    Wave wav;

    f.read((char *)&wav._riff_tag,      4);
    f.read((char *)&wav._riff_length,   4);
    f.read((char *)&wav._wave_tag,      4);
    f.read((char *)&wav._fmt_tag,       4);
    f.read((char *)&wav._fmt_length,    4);
    f.read((char *)&wav._audio_format,  2);
    f.read((char *)&wav._num_channels,  2);
    f.read((char *)&wav._sample_rate,   4);
    f.read((char *)&wav._byte_rate,     4);
    f.read((char *)&wav._block_align,   2);
    f.read((char *)&wav._bits_per_sample, 2);
    qint16 cbSize;
    f.read((char *)&cbSize, 2);
    if (cbSize)
        f.read(22);
    f.read((char *)&wav._data_tag, 4);
    f.read((char *)&wav._data_length, 4);

    QByteArray data;
    data.resize(wav._data_length);
    float *sampleData = (float *)data.data();
    f.read(data.data(), wav._data_length);


    int bytes_per_sample = wav._bits_per_sample / 8;
    int samples = wav._data_length / wav._num_channels / bytes_per_sample;
    wav._samplesPerChannel = samples;
    wav._bytesPerSample = bytes_per_sample;

    if (bytes_per_sample != 4)
        throw new std::exception("Must use an 32-bit float wave file");

    for (int i = 0; i < wav._num_channels; ++i) {
        Ipp32f *channel_data = (Ipp32f *)ippsMalloc_32f(samples);
        wav._data.push_back(channel_data);
        for (int j = 0; j < samples; ++j) {
            channel_data[j] = sampleData[(j * wav._num_channels + i)];
        }
    }

    return wav;
}

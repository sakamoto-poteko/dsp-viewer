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


#ifndef WAVE_H
#define WAVE_H

#include <vector>

#include <QtGlobal>

#include <ipp.h>


class Wave
{
public:
    Wave();
    ~Wave();

    static Wave read(const QString &path);

    float *data(int channel)
    {
        return _data.at(channel);
    }

    int numChannels()           { return _num_channels; }
    int bytesPerSample()        { return _bytesPerSample; }
    int numSamplesPerChannel()  { return _samplesPerChannel; }
    int sampleRate()            { return _sample_rate; }

private:
    qint32  _riff_tag;
    qint32	_riff_length;
    qint32	_wave_tag;
    qint32	_fmt_tag;
    qint32	_fmt_length;
    qint16	_audio_format;
    qint16	_num_channels;
    qint32	_sample_rate;
    qint32	_byte_rate;
    qint16	_block_align;
    qint16	_bits_per_sample;
    qint32	_data_tag;
    qint32	_data_length;

    qint32  _samplesPerChannel;
    qint32  _bytesPerSample;

    std::vector<Ipp32f *> _data;
};

#endif // WAVE_H

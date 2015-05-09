#include "FFTScaleDraw.h"


FFTScaleDraw::FFTScaleDraw(int sampleRate, int fftWindow) :
    _fftWindow(fftWindow), _sampleRate(sampleRate)
{
}


FFTScaleDraw::~FFTScaleDraw()
{
}

QwtText FFTScaleDraw::label(double value) const
{
    return QString("%1 kHz (%2)").arg(QString::number(value * _sampleRate / _fftWindow / 1000, 'g', 3)).arg(value);
}
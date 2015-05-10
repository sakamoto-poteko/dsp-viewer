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


#ifndef FFTSPECTRUMWIDGET_H
#define FFTSPECTRUMWIDGET_H

#include <QLayout>
#include "PlotBaseWidget.h"

#include <qwt_plot_spectrogram.h>
#include <qwt_color_map.h>

#include "FFTScaleDraw.h"
#include "TransposedMatrixRasterData.h"
#include "shared_Ipp_ptr.h"

class FFTSpectrumWidget : public PlotBaseWidget
{
    Q_OBJECT
public:
    explicit FFTSpectrumWidget(bool logColorScale = true, const QString &title = "", QWidget *parent = 0);
    ~FFTSpectrumWidget();

    // Usually, x is times performed fft, y is N / 2. Data size = x * y
    void setData(const shared_Ipp_ptr<Ipp32f> &data, int x_len, int y_len, int sampleRate = 0, int fftWindow = 0);

signals:

public slots:

protected:
    QwtPlotSpectrogram *spectrumPlot;
    QwtLinearColorMap *colorMap;
    TransposedMatrixRasterData<float> *spectrumPlotData;
    FFTScaleDraw *plotYAxisScale;


    shared_Ipp_ptr<Ipp32f> _data;
};

#endif // FFTSPECTRUMWIDGET_H

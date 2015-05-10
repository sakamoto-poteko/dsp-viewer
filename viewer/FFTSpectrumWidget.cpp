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


#include "FFTSpectrumWidget.h"

FFTSpectrumWidget::FFTSpectrumWidget(bool logColorScale, const QString &title, QWidget *parent) 
    : PlotBaseWidget(title, parent)
{
    // Plot Content
    spectrumPlot = new QwtPlotSpectrogram;
    spectrumPlot->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    spectrumPlot->attach(plot);

    // Plot Data
    spectrumPlotData = new TransposedMatrixRasterData<>;

    // Color Map
    colorMap = new QwtLinearColorMap;
    if (logColorScale) {
        colorMap->addColorStop(0,     Qt::black);
        colorMap->addColorStop(1e-11, QColor(0,   0,   255));
        colorMap->addColorStop(1e-10, QColor(0,   127, 255));
        colorMap->addColorStop(1e-9,  QColor(0,   255, 255));
        colorMap->addColorStop(1e-8,  QColor(127, 0,   255));
        colorMap->addColorStop(1e-7,  QColor(255, 0,   255));
        colorMap->addColorStop(1e-6,  QColor(255, 0,   127));
        colorMap->addColorStop(1e-5,  QColor(255, 0,   0));
        colorMap->addColorStop(1e-4,  QColor(255, 127, 0));
        colorMap->addColorStop(1e-3,  QColor(255, 255, 0));
        colorMap->addColorStop(1e-2,  QColor(127, 255, 0));
        colorMap->addColorStop(1e-1,  QColor(0,   255, 0));
        colorMap->addColorStop(1e+0,  QColor(0,   255, 127));
    } else {
        colorMap->addColorStop(0.0, Qt::black);
        colorMap->addColorStop(0.1, QColor(0,   0,   255));
        colorMap->addColorStop(0.2, QColor(127, 0,   255));
        colorMap->addColorStop(0.3, QColor(255, 0,   255));
        colorMap->addColorStop(0.4, QColor(255, 0,   127));
        colorMap->addColorStop(0.5, QColor(255, 0,   0));
        colorMap->addColorStop(0.6, QColor(255, 127, 0));
        colorMap->addColorStop(0.7, QColor(255, 255, 0));
        colorMap->addColorStop(0.8, QColor(127, 255, 0));
        colorMap->addColorStop(0.9, QColor(0,   255, 0));
        colorMap->addColorStop(1.0, QColor(0,   255, 127));
    }
    colorMap->setMode(QwtLinearColorMap::ScaledColors);
    spectrumPlot->setColorMap(colorMap);

    // Scale
    plotYAxisScale = new FFTScaleDraw;
    plot->setAxisScaleDraw(QwtPlot::yLeft, plotYAxisScale);
}

FFTSpectrumWidget::~FFTSpectrumWidget()
{
}

void FFTSpectrumWidget::setData(const shared_Ipp_ptr<Ipp32f> &data, int x_len, int y_len, int sampleRate, int fftWindow)
{
    _data = data;

    if (sampleRate) {
        plotYAxisScale->setParameter(sampleRate, fftWindow);
    }
    spectrumPlotData->setValueMatrix(data.get(), y_len, x_len);

    spectrumPlotData->setInterval(Qt::XAxis, QwtInterval(0, x_len));
    plot->setAxisScale(QwtPlot::xBottom, 0, x_len);

    spectrumPlotData->setInterval(Qt::YAxis, QwtInterval(0, y_len));
    plot->setAxisScale(QwtPlot::yLeft, 0, y_len);

    spectrumPlotData->setInterval(Qt::ZAxis, QwtInterval(0, 1));
    spectrumPlot->setData(spectrumPlotData);

    plotZoomer->setZoomBase();
}

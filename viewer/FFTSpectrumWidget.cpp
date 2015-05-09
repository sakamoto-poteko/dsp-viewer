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

FFTSpectrumWidget::FFTSpectrumWidget(bool logColorScale, QWidget *parent) : QWidget(parent)
{
    mainLayout = new QHBoxLayout;

    // Plot
    plot = new QwtPlot(this);

    // Plot Content
    spectrumPlot = new QwtPlotSpectrogram;
    spectrumPlot->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    spectrumPlot->attach(plot);

    // Plot Data
    spectrumPlotData = new TransposedMatrixRasterData<>;

    // Color Map
    colorMap = new QwtLinearColorMap;
    if (logColorScale) {
        colorMap->addColorStop(0, Qt::black);
        colorMap->addColorStop(1e-6, Qt::darkBlue);
        colorMap->addColorStop(1e-5, Qt::blue);
        colorMap->addColorStop(1e-4, Qt::cyan);
        colorMap->addColorStop(1e-3, Qt::green);
        colorMap->addColorStop(1e-2, Qt::yellow);
        colorMap->addColorStop(1e-1, Qt::magenta);
        colorMap->addColorStop(1e0, Qt::red);
    } else {
        colorMap->addColorStop(0, Qt::black);
        colorMap->addColorStop(0.33, Qt::blue);
        colorMap->addColorStop(0.66, Qt::red);
        colorMap->addColorStop(1, Qt::yellow);
    }
    colorMap->setMode(QwtLinearColorMap::ScaledColors);
    spectrumPlot->setColorMap(colorMap);
    
    // Scale
    plotYAxisScale = new FFTScaleDraw;
    plot->setAxisScaleDraw(QwtPlot::yLeft, plotYAxisScale);

    // Zoomer
    plotZoomer = new QwtPlotZoomer(plot->canvas());
    plotZoomer->setZoomBase();
    plotZoomer->setRubberBandPen(QPen(Qt::white));
    plotZoomer->setTrackerPen(QPen(Qt::white));

    mainLayout->addWidget(plot);
    setLayout(mainLayout);
}

FFTSpectrumWidget::~FFTSpectrumWidget()
{
}

void FFTSpectrumWidget::setData(const shared_Ipp_ptr<Ipp32f> &data, int x, int y, int sampleRate, int fftWindow)
{
    _data = data;

    if (sampleRate) {
        plotYAxisScale->setParameter(sampleRate, fftWindow);
    }
    spectrumPlotData->setValueMatrix(data.get(), y, x);

    spectrumPlotData->setInterval(Qt::XAxis, QwtInterval(0, x));
    plot->setAxisScale(QwtPlot::xBottom, 0, x);

    spectrumPlotData->setInterval(Qt::YAxis, QwtInterval(0, y));
    plot->setAxisScale(QwtPlot::yLeft, 0, y);

    spectrumPlotData->setInterval(Qt::ZAxis, QwtInterval(0, 100));
    spectrumPlot->setData(spectrumPlotData);

    plotZoomer->setZoomBase();
}

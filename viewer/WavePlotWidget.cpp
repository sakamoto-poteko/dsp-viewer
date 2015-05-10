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

#include <ipp.h>
#include "WavePlotWidget.h"


WavePlotWidget::WavePlotWidget(const QString &title, const QPen &pen, QWidget *parent) : PlotBaseWidget(title, parent)
{
    plotCurve = new QwtPlotCurve;
    plotCurve->setPen(pen);
    plotCurve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    plotCurve->attach(plot);
}

WavePlotWidget::~WavePlotWidget()
{
    plotCurve->setSamples(QVector<QPointF>());
}

void WavePlotWidget::setData(const shared_Ipp_ptr<Ipp32f> &data, int dataLen, float x_min, float x_max)
{
    _xData = shared_Ipp_ptr<Ipp64f>(dataLen);
    _yData = shared_Ipp_ptr<Ipp64f>(dataLen);
    plotCurve->setRawSamples(_xData.get(), _yData.get(), dataLen);

    float x_step = (x_max - x_min) / dataLen;
    for (int i = 0; i < dataLen; ++i) {
        _xData.get()[i] = i * x_step;
    }
    ippsConvert_32f64f(data.get(), _yData.get(), dataLen);
    resetZoom();
}

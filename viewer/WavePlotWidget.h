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


#ifndef WAVEPLOTWIDGET_H
#define WAVEPLOTWIDGET_H

#include <QWidget>
#include "PlotBaseWidget.h"

#include <qwt_plot_curve.h>

#include "shared_Ipp_ptr.h"

class WavePlotWidget : public PlotBaseWidget
{
    Q_OBJECT
public:
    explicit WavePlotWidget(const QString &title = "", const QPen &pen = QPen(Qt::black), QWidget *parent = 0);
    ~WavePlotWidget();

    void setData(const shared_Ipp_ptr<Ipp32f> &data, int dataLen, float x_min, float x_max);
signals:

public slots:

protected:
    QwtPlotCurve *plotCurve;

    shared_Ipp_ptr<Ipp64f> _xData;
    shared_Ipp_ptr<Ipp64f> _yData;
};

#endif // WAVEPLOTWIDGET_H

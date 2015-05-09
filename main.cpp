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


#include <QApplication>
#include <QLayout>
#include <QWidget>
#include <QVector>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_scale_engine.h>
#include <qwt_plot_spectrogram.h>
#include <qwt_matrix_raster_data.h>
#include <qwt_color_map.h>
#include <qwt_plot_zoomer.h>

#include <ipp.h>
#include <ippvm.h>

#include <omp.h>

#include "wave.h"
#include "FFTCalculator.h"
#include "FFTSpectrumWidget.h"

#ifdef NDEBUG
#pragma comment(lib, "qwt")
#else
#pragma comment(lib, "qwtd")
#endif


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ippInit();
    //Set the size
    const size_t N = 1024;  // 48kHz / 1024 = 46.88Hz, 48kHz * 1024 = 0.02s

    Wave wav = Wave::read("D:/patternRecog/yui3f32.wav");
    Ipp32f *channel1 = wav.channelData(0);

    FFTSpectrumWidget *window = new FFTSpectrumWidget;

    int fft_loop = wav.numSamplesPerChannel() - N;
    shared_Ipp_ptr<Ipp32f> fftResult = shared_Ipp_ptr<Ipp32f>(N / 2 * fft_loop);

    FFTCalculator calculator(N);

    int numThreads = omp_get_max_threads();
    std::vector<Ipp32f *> dstWorkBuf;
    std::vector<Ipp8u *> fftWorkBuf;
    std::vector<Ipp32f *> pSrc;
    for (int i = 0; i < numThreads; ++i) {
        pSrc.push_back(ippsMalloc_32f(N));
        dstWorkBuf.push_back(ippsMalloc_32f(N * 2));
        fftWorkBuf.push_back(ippsMalloc_8u(calculator.getSizeFFTWorkBuf()));
    }


#pragma omp parallel for
    for (int i = 0; i < fft_loop; ++i) {
        int threadIdx = omp_get_thread_num();
        memcpy(pSrc.at(threadIdx), channel1 + i, N * sizeof(float));
        calculator.FFT_r(pSrc.at(threadIdx), 
                         fftResult.get() + (N / 2) * i, 
                         dstWorkBuf.at(threadIdx), 
                         fftWorkBuf.at(threadIdx));
    }

    FFTCalculator::Normalize(fftResult.get(), N / 2 * fft_loop);
    window->setData(fftResult, fft_loop, N / 2, wav.sampleRate(), N);

    for (auto workBufItem : dstWorkBuf)
        ippFree(workBufItem);
    for (auto fftWorkBufItem : fftWorkBuf)
        ippFree(fftWorkBufItem);
    for (auto pSrcItem : pSrc)
        ippFree(pSrcItem);

    window->showMaximized();
    return a.exec();
}



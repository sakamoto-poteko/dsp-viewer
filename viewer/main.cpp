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

#include <ipp.h>
#include <ippvm.h>

#include <omp.h>

#include "wave.h"
#include "FFTCalculator.h"
#include "FFTSpectrumWidget.h"
#include "SRFilter.h"
#include "WavePlotWidget.h"

#ifdef NDEBUG
#pragma comment(lib, "qwt")
#else
#pragma comment(lib, "qwtd")
#endif


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // Intel IPP
    ippInit();
    // OpenMP
    int numThreads = omp_get_max_threads();

    // Read file
    Wave wav = Wave::read("D:/parr/yui3f32.wav");
    Ipp32f *sourceSamples = wav.data(0);

    // High Pass Filter
    shared_Ipp_ptr<Ipp32f> tap(2);
    tap.get()[0] = 1.f; tap.get()[1] = -0.95f;
    SRFilter filter(tap, 2);
    Ipp32f *filteredSamples = ippsMalloc_32f(wav.numSamplesPerChannel());
    filter.Filter(sourceSamples, filteredSamples, wav.numSamplesPerChannel());
    
    FFTSpectrumWidget *window = new FFTSpectrumWidget(true, "Filtered");

    // FFT
    // Window size
    const size_t N = 2048;  // 48kHz / 1024 = 23.44Hz, 48kHz * 2048 = 0.043s
    int fftIterations = wav.numSamplesPerChannel() - N;
    // FFT result
    shared_Ipp_ptr<Ipp32f> fftSpectrum = shared_Ipp_ptr<Ipp32f>(N / 2 * fftIterations);

    FFTCalculator calculator(N);

    // Allocate FFT buffers (for multithreaded version)
    std::vector<Ipp32f *> dstWorkBuf;
    std::vector<Ipp8u *> fftWorkBuf;
    std::vector<Ipp32f *> pSrc;
    for (int i = 0; i < numThreads; ++i) {
        pSrc.push_back(ippsMalloc_32f(N));
        dstWorkBuf.push_back(ippsMalloc_32f(N * 2));
        fftWorkBuf.push_back(ippsMalloc_8u(calculator.getSizeWorkBuf()));
    }

    // Perform FFT
#pragma omp parallel for
    for (int i = 0; i < fftIterations; ++i) {
        int threadIdx = omp_get_thread_num();
        memcpy(pSrc.at(threadIdx), filteredSamples + i, N * sizeof(float));
        calculator.FFT_r(pSrc.at(threadIdx), 
                         fftSpectrum.get() + (N / 2) * i, 
                         dstWorkBuf.at(threadIdx), 
                         fftWorkBuf.at(threadIdx));
    }

    // Normalize FFT result
    FFTCalculator::Normalize(fftSpectrum.get(), N / 2 * fftIterations);
    // Display FFT spectrum
    window->setData(fftSpectrum.clone(), fftIterations, N / 2, wav.sampleRate(), N);

    WavePlotWidget *singleSpec = new WavePlotWidget("Plot");
    singleSpec->setData(shared_Ipp_ptr<Ipp32f>(fftSpectrum.get() + 26245 * (N / 2), N / 2).clone(), N / 2, 0, N / 2 * wav.sampleRate() / N);
    singleSpec->useLogScale(QwtPlot::yLeft);
    singleSpec->showMaximized();

    /*
    WavePlotWidget *invfft = new WavePlotWidget("Inverse");
    FFTCalculator invFFTCalc(N / 2);
    Ipp32f *srcVec = ippsMalloc_32f(N / 2);
    ippsLog10_32f_A24(fftSpectrum.get() + 26245, srcVec, N / 2);
    //memcpy(srcVec, fftSpectrum.get() + 26245, N / 2);
    shared_Ipp_ptr<Ipp32f> dstVec(N / 2);
    invFFTCalc.InvFFT(srcVec, dstVec.get());
    invfft->setData(dstVec, N / 2, 0, N / 2 * wav.sampleRate() / N);
    //invfft->useLogScale(QwtPlot::xBottom);
    invfft->showMaximized();
    */


    // Clean up
    for (auto workBufItem : dstWorkBuf)
        ippFree(workBufItem);
    for (auto fftWorkBufItem : fftWorkBuf)
        ippFree(fftWorkBufItem);
    for (auto pSrcItem : pSrc)
        ippFree(pSrcItem);
    
    // Show window
    window->showMaximized();
    return a.exec();
}



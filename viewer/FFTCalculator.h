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

#ifndef FFTCALCULATOR_H
#define FFTCALCULATOR_H

#include <functional>

#include <ipp.h>

class FFTCalculator
{
public:
    // Default window function is Hamming
    FFTCalculator(int N, std::function<void(const Ipp32f*, Ipp32f*, int)> windowFunc = 
        [](const Ipp32f *in, Ipp32f *out, int n) {
            ippsWinHamming_32f(in, out, n);
        });
    ~FFTCalculator();

    // src: size N, dst: size N / 2
    void FFT(const Ipp32f *src, Ipp32f *dst);
    // src: size N, dst: size N
    void InvFFT(const Ipp32f *src, Ipp32f *dst);
    // src: size N, dst: size N / 2, dstWorkBuf: size N * 2, fftWorkBuf: size getSizeWorkBuf()
    void FFT_r(const Ipp32f *src, Ipp32f *dst, Ipp32f *dstWorkBuf, Ipp8u *fftWorkBuf);
    // src: size N, dst: size N, dstWorkBuf: size N * 2, fftWorkBuf: size getSizeWorkBuf()
    void InvFFT_r(const Ipp32f *src, Ipp32f *dst, Ipp32f *dstWorkBuf, Ipp8u *fftWorkBuf);

    int getSizeWorkBuf() 
    {
        return sizeFFTWorkBuf; 
    }

    void setWindowFunction(std::function<void(const Ipp32f*, Ipp32f*, int)> windowFunc)
    {
        windowFunction = windowFunc;
    }

    static void Normalize(Ipp32f *src, int len);

protected:
    int _N;

    IppsFFTSpec_C_32f *pFFTSpec = 0;
    Ipp8u *pFFTSpecBuf = 0;
    Ipp8u *pFFTInitBuf = 0;
    Ipp8u *pFFTWorkBuf = 0;
    Ipp32f *pDst = 0;

    int sizeFFTSpec;
    int sizeFFTInitBuf;
    int sizeFFTWorkBuf;

    std::function<void(const Ipp32f*, Ipp32f*, int)> windowFunction;
};



#endif
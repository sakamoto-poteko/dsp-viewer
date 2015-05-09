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

#include <ipp.h>

class FFTCalculator
{
public:
    FFTCalculator(int N);
    ~FFTCalculator();

    // src: size N, dst: size N / 2
    void FFT(const Ipp32f *src, Ipp32f *dst);
    // src: size N, dst: size N / 2, dstWorkBuf: size N * 2, fftWorkBuf: size sizeFFTWorkBuf
    void FFT_r(const Ipp32f *src, Ipp32f *dst, Ipp32f *dstWorkBuf, Ipp8u *fftWorkBuf);

    int getSizeFFTWorkBuf() { return sizeFFTWorkBuf; }

    static void Normalize(Ipp32f *src, size_t len);

protected:
    int _N;

    IppsFFTSpec_C_32f *pFFTSpec = 0;
    Ipp8u *pFFTSpecBuf = 0;
    Ipp8u *pFFTInitBuf = 0;
    Ipp8u *pFFTWorkBuf = 0;
    Ipp32f *pSrcIm = 0;
    Ipp32f *pDst = 0;
    Ipp32f *pDstIm = 0;

    int sizeFFTSpec;
    int sizeFFTInitBuf;
    int sizeFFTWorkBuf;
};



#endif
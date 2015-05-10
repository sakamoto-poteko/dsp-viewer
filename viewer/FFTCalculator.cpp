#include "FFTCalculator.h"

#include <cmath>
#include <cstring>

FFTCalculator::FFTCalculator(int N, std::function<void(const Ipp32f*, Ipp32f*, int)> windowFunc) :
    _N(N), windowFunction(windowFunc)
{
    const int order = (int)(log((double)N) / log(2.0));

    pDst = ippsMalloc_32f(N * 2);

    ippsFFTGetSize_C_32f(order, IPP_FFT_NODIV_BY_ANY,
        ippAlgHintAccurate, &sizeFFTSpec, &sizeFFTInitBuf, &sizeFFTWorkBuf);

    pFFTSpecBuf = ippsMalloc_8u(sizeFFTSpec);
    pFFTInitBuf = ippsMalloc_8u(sizeFFTInitBuf);
    pFFTWorkBuf = ippsMalloc_8u(sizeFFTWorkBuf);

    ippsFFTInit_C_32f(&pFFTSpec, order, IPP_FFT_NODIV_BY_ANY,
        ippAlgHintAccurate, pFFTSpecBuf, pFFTInitBuf);

    if (pFFTInitBuf) ippFree(pFFTInitBuf);
}


FFTCalculator::~FFTCalculator()
{
    if (pFFTWorkBuf) ippFree(pFFTWorkBuf);
    if (pFFTSpecBuf) ippFree(pFFTSpecBuf);

    ippFree(pDst);
}

// This function is non-reentrant
void FFTCalculator::FFT(const Ipp32f *src, Ipp32f *dst)
{
    windowFunction(src, pDst, _N);
    std::memset(pDst + _N, 0, _N * sizeof(Ipp32f));
    ippsFFTFwd_CToC_32f_I(pDst, pDst + _N, pFFTSpec, pFFTWorkBuf);

    ippsMagnitude_32f(pDst, pDst + _N, dst, _N / 2);
}

// This function is non-reentrant
void FFTCalculator::InvFFT(const Ipp32f *src, Ipp32f *dst)
{
    std::memcpy(pDst, src, _N * sizeof(Ipp32f));
    std::memset(pDst + _N, 0, _N * sizeof(Ipp32f));
    ippsFFTInv_CToC_32f_I(pDst, pDst + _N, pFFTSpec, pFFTWorkBuf);

    // Copies only real part
    std::memcpy(dst, pDst, _N * sizeof(Ipp32f));
}

void FFTCalculator::FFT_r(const Ipp32f *src, Ipp32f *dst, Ipp32f *dstWorkBuf, Ipp8u *fftWorkBuf)
{
    windowFunction(src, dstWorkBuf, _N);
    std::memset(dstWorkBuf + _N, 0, _N * sizeof(Ipp32f));
    ippsFFTFwd_CToC_32f_I(dstWorkBuf, dstWorkBuf + _N, pFFTSpec, fftWorkBuf);

    ippsMagnitude_32f(dstWorkBuf, dstWorkBuf + _N, dst, _N / 2);
}

void FFTCalculator::InvFFT_r(const Ipp32f *src, Ipp32f *dst, Ipp32f *dstWorkBuf, Ipp8u *fftWorkBuf)
{
    std::memcpy(dstWorkBuf, src, _N);
    std::memset(dstWorkBuf + _N, 0, _N * sizeof(Ipp32f));
    ippsFFTInv_CToC_32f_I(dstWorkBuf, dstWorkBuf + _N, pFFTSpec, fftWorkBuf);

    // Copies only real part
    std::memcpy(dst, dstWorkBuf, _N * sizeof(Ipp32f));
}

void FFTCalculator::Normalize(Ipp32f *src, int len)
{
    Ipp32f min, max;
    ippsMinMax_32f(src, len, &min, &max);
    ippsNormalize_32f(src, src, len, min, max - min);
}

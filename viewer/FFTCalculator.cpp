#include "FFTCalculator.h"

#include <cmath>
#include <cstring>

FFTCalculator::FFTCalculator(int N)
{
    const int order = (int)(log((double)N) / log(2.0));
    _N = N;

    pSrcIm = ippsMalloc_32f(N);
    std::memset(pSrcIm, 0, N * sizeof(Ipp32f));
    pDst = ippsMalloc_32f(N);
    pDstIm = ippsMalloc_32f(N);

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

    ippFree(pSrcIm);
    ippFree(pDst);
    ippFree(pDstIm);
}

// This function is non-reentrant
void FFTCalculator::FFT(const Ipp32f *src, Ipp32f *dst)
{
    ippsFFTFwd_CToC_32f(src, pSrcIm, pDst, pDstIm, pFFTSpec, pFFTWorkBuf);

    ippsPowerSpectr_32f(pDst, pDstIm, dst, _N / 2);
    ippsSqrt_32f_I(dst, _N / 2);
}

void FFTCalculator::FFT_r(const Ipp32f *src, Ipp32f *dst, Ipp32f *dstWorkBuf, Ipp8u *fftWorkBuf)
{
    ippsFFTFwd_CToC_32f(src, pSrcIm, dstWorkBuf, dstWorkBuf + _N, pFFTSpec, fftWorkBuf);

    ippsPowerSpectr_32f(dstWorkBuf, dstWorkBuf + _N, dst, _N / 2);
    ippsSqrt_32f_I(dst, _N / 2);
}

void FFTCalculator::Normalize(Ipp32f *src, int len)
{
    Ipp32f min, max;
    ippsMinMax_32f(src, len, &min, &max);
    ippsNormalize_32f(src, src, len, min, max - min);
}

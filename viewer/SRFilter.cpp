#include "SRFilter.h"


SRFilter::SRFilter(shared_Ipp_ptr<Ipp32f> taps, int tapsLen) :
    _taps(taps), _tapsLen(tapsLen)
{
    int specSize;
    ippsFIRSRGetSize(tapsLen, ipp32f, &specSize, &sizeWorkBuf);
    workBuf = ippsMalloc_8u(sizeWorkBuf);
    pSpec = (IppsFIRSpec_32f*)ippsMalloc_8u(specSize);
    ippsFIRSRInit_32f(taps.get(), tapsLen, ippAlgDirect, pSpec);
}


SRFilter::~SRFilter()
{
    ippFree(workBuf);
    ippFree(pSpec);
}

void SRFilter::Filter(const Ipp32f *src, Ipp32f *dst, int len)
{
    ippsFIRSR_32f(src, dst, len, pSpec, NULL, NULL, workBuf);
}

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

#ifndef SHARED_IPP_PTR_H
#define SHARED_IPP_PTR_H

#include <memory>

#include <ipp.h>

template <typename T>
class shared_Ipp_ptr : public std::shared_ptr<T>
{
public:
    shared_Ipp_ptr(size_t size = 0) :
        std::shared_ptr<T>(size ? (T *)_mm_malloc(size * sizeof(T), 64) : 0, 
        [](T *dPtr){
            if (dPtr) { _mm_free(dPtr); dPtr = 0; }
        }), _size(size)
    {
    }

    shared_Ipp_ptr(T *ptr, size_t size) :
        std::shared_ptr<T>(ptr, [](T *dPtr){}), _size(size)
    {
    }

    shared_Ipp_ptr clone()
    {
        shared_Ipp_ptr cloned = shared_Ipp_ptr(_size);
        std::memcpy(cloned.get(), get(), _size * sizeof(T));
        return cloned;
    }

    static shared_Ipp_ptr make_clone(T *ptr, size_t size)
    {
        shared_Ipp_ptr cloned(size);
        std::memcpy(cloned.ptr, get(), size * sizeof(T));
        return cloned;
    }

protected:
    size_t _size;
};


#endif
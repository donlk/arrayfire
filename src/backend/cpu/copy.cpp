/*******************************************************
 * Copyright (c) 2014, ArrayFire
 * All rights reserved.
 *
 * This file is distributed under 3-clause BSD license.
 * The complete license agreement can be obtained at:
 * http://arrayfire.com/licenses/BSD-3-Clause
 ********************************************************/

#include <type_traits>
#include <Array.hpp>
#include <copy.hpp>
#include <cstring>
#include <algorithm>
#include <complex>
#include <vector>
#include <cassert>
#include <err_cpu.hpp>
#include <math.hpp>
#include <platform.hpp>
#include <queue.hpp>
#include <kernel/copy.hpp>

namespace cpu
{

template<typename T>
void copyData(T *to, const Array<T> &from)
{
    from.eval();
    getQueue().sync();
    if(from.isLinear()) {
        // FIXME: Check for errors / exceptions
        memcpy(to, from.get(), from.elements()*sizeof(T));
    } else {
        dim4 ostrides = calcStrides(from.dims());
        kernel::stridedCopy<T>(to, ostrides, from.get(), from.dims(), from.strides(), from.ndims() - 1);
    }
}

template<typename T>
Array<T> copyArray(const Array<T> &A)
{
    A.eval();
    Array<T> out = createEmptyArray<T>(A.dims());
    getQueue().enqueue(kernel::copy<T, T>, out, A);
    return out;
}

template<typename inType, typename outType>
void copyArray(Array<outType> &out, Array<inType> const &in)
{
    out.eval();
    in.eval();
    getQueue().enqueue(kernel::copy<outType, inType>, out, in);
}

#define INSTANTIATE(T)                                                  \
    template void      copyData<T> (T *data, const Array<T> &from);     \
    template Array<T>  copyArray<T>(const Array<T> &A);                 \

INSTANTIATE(float  )
INSTANTIATE(double )
INSTANTIATE(cfloat )
INSTANTIATE(cdouble)
INSTANTIATE(int    )
INSTANTIATE(uint   )
INSTANTIATE(uchar  )
INSTANTIATE(char   )
INSTANTIATE(intl   )
INSTANTIATE(uintl  )
INSTANTIATE(short  )
INSTANTIATE(ushort )

#define INSTANTIATE_COPY_ARRAY(SRC_T)                                    \
    template void copyArray<SRC_T, float  >(Array<float  > &dst, Array<SRC_T> const &src);  \
    template void copyArray<SRC_T, double >(Array<double > &dst, Array<SRC_T> const &src);  \
    template void copyArray<SRC_T, cfloat >(Array<cfloat > &dst, Array<SRC_T> const &src);  \
    template void copyArray<SRC_T, cdouble>(Array<cdouble> &dst, Array<SRC_T> const &src);  \
    template void copyArray<SRC_T, int    >(Array<int    > &dst, Array<SRC_T> const &src);  \
    template void copyArray<SRC_T, uint   >(Array<uint   > &dst, Array<SRC_T> const &src);  \
    template void copyArray<SRC_T, intl   >(Array<intl   > &dst, Array<SRC_T> const &src);  \
    template void copyArray<SRC_T, uintl  >(Array<uintl  > &dst, Array<SRC_T> const &src);  \
    template void copyArray<SRC_T, short  >(Array<short  > &dst, Array<SRC_T> const &src);  \
    template void copyArray<SRC_T, ushort >(Array<ushort > &dst, Array<SRC_T> const &src);  \
    template void copyArray<SRC_T, uchar  >(Array<uchar  > &dst, Array<SRC_T> const &src);  \
    template void copyArray<SRC_T, char   >(Array<char   > &dst, Array<SRC_T> const &src);

INSTANTIATE_COPY_ARRAY(float )
INSTANTIATE_COPY_ARRAY(double)
INSTANTIATE_COPY_ARRAY(int   )
INSTANTIATE_COPY_ARRAY(uint  )
INSTANTIATE_COPY_ARRAY(intl  )
INSTANTIATE_COPY_ARRAY(uintl )
INSTANTIATE_COPY_ARRAY(uchar )
INSTANTIATE_COPY_ARRAY(char  )
INSTANTIATE_COPY_ARRAY(ushort)
INSTANTIATE_COPY_ARRAY(short )

#define INSTANTIATE_COPY_ARRAY_COMPLEX(SRC_T)                            \
    template void copyArray<SRC_T, cfloat  >(Array<cfloat  > &dst, Array<SRC_T> const &src);    \
    template void copyArray<SRC_T, cdouble   >(Array<cdouble > &dst, Array<SRC_T> const &src);

INSTANTIATE_COPY_ARRAY_COMPLEX(cfloat )
INSTANTIATE_COPY_ARRAY_COMPLEX(cdouble)

#define SPECILIAZE_UNUSED_COPYARRAY(SRC_T, DST_T) \
    template<> void copyArray<SRC_T, DST_T>(Array<DST_T> &out, Array<SRC_T> const &in) \
    {\
        CPU_NOT_SUPPORTED();\
    }

SPECILIAZE_UNUSED_COPYARRAY(cfloat , double)
SPECILIAZE_UNUSED_COPYARRAY(cfloat , float)
SPECILIAZE_UNUSED_COPYARRAY(cfloat , uchar)
SPECILIAZE_UNUSED_COPYARRAY(cfloat , char)
SPECILIAZE_UNUSED_COPYARRAY(cfloat , uint)
SPECILIAZE_UNUSED_COPYARRAY(cfloat , int)
SPECILIAZE_UNUSED_COPYARRAY(cfloat , intl)
SPECILIAZE_UNUSED_COPYARRAY(cfloat , uintl)
SPECILIAZE_UNUSED_COPYARRAY(cfloat , short)
SPECILIAZE_UNUSED_COPYARRAY(cfloat , ushort)
SPECILIAZE_UNUSED_COPYARRAY(cdouble, double)
SPECILIAZE_UNUSED_COPYARRAY(cdouble, float)
SPECILIAZE_UNUSED_COPYARRAY(cdouble, uchar)
SPECILIAZE_UNUSED_COPYARRAY(cdouble, char)
SPECILIAZE_UNUSED_COPYARRAY(cdouble, uint)
SPECILIAZE_UNUSED_COPYARRAY(cdouble, int)
SPECILIAZE_UNUSED_COPYARRAY(cdouble, intl)
SPECILIAZE_UNUSED_COPYARRAY(cdouble, uintl)
SPECILIAZE_UNUSED_COPYARRAY(cdouble, short)
SPECILIAZE_UNUSED_COPYARRAY(cdouble, ushort)

}

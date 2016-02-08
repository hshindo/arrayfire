/*******************************************************
 * Copyright (c) 2014, ArrayFire
 * All rights reserved.
 *
 * This file is distributed under 3-clause BSD license.
 * The complete license agreement can be obtained at:
 * http://arrayfire.com/licenses/BSD-3-Clause
 ********************************************************/

#include <Array.hpp>
#include <join.hpp>
#include <platform.hpp>
#include <queue.hpp>
#include <kernel/join.hpp>
#include <boost/preprocessor.hpp>

namespace cpu
{

template<typename Tx, typename Ty>
Array<Tx> join(const int dim, const Array<Tx> &first, const Array<Ty> &second)
{
    first.eval();
    second.eval();

    // All dimensions except join dimension must be equal
    // Compute output dims
    af::dim4 odims;
    af::dim4 fdims = first.dims();
    af::dim4 sdims = second.dims();

    for(int i = 0; i < 4; i++) {
        if(i == dim) {
            odims[i] = fdims[i] + sdims[i];
        } else {
            odims[i] = fdims[i];
        }
    }

    Array<Tx> out = createEmptyArray<Tx>(odims);

    getQueue().enqueue(kernel::join<Tx, Ty>, out, dim, first, second);

    return out;
}

#define JOINCASE(Z, N, D) \
	case N: \
		getQueue().enqueue(kernel::join<T, N>, dim, out, inputs); \
		break;

template<typename T>
Array<T> join(const int dim, const std::vector<Array<T>> &inputs)
{
    for (unsigned i=0; i<inputs.size(); ++i)
        inputs[i].eval();
    // All dimensions except join dimension must be equal
    // Compute output dims
    af::dim4 odims;
    const dim_t n_arrays = inputs.size();
    std::vector<af::dim4> idims(n_arrays);

    dim_t dim_size = 0;
    for(unsigned i = 0; i < idims.size(); i++) {
        idims[i] = inputs[i].dims();
        dim_size += idims[i][dim];
    }

    for(int i = 0; i < 4; i++) {
        if(i == dim) {
            odims[i] = dim_size;
        } else {
            odims[i] = idims[0][i];
        }
    }

    Array<T> out = createEmptyArray<T>(odims);

    switch(n_arrays) {
		BOOST_PP_REPEAT(200, JOINCASE, _)
    }

    return out;
}

#define INSTANTIATE(Tx, Ty) \
    template Array<Tx> join<Tx, Ty>(const int dim, const Array<Tx> &first, const Array<Ty> &second);

INSTANTIATE(float,   float)
INSTANTIATE(double,  double)
INSTANTIATE(cfloat,  cfloat)
INSTANTIATE(cdouble, cdouble)
INSTANTIATE(int,     int)
INSTANTIATE(uint,    uint)
INSTANTIATE(intl,    intl)
INSTANTIATE(uintl,   uintl)
INSTANTIATE(uchar,   uchar)
INSTANTIATE(char,    char)
INSTANTIATE(ushort,  ushort)
INSTANTIATE(short,   short)

#undef INSTANTIATE

#define INSTANTIATE(T)      \
    template Array<T> join<T>(const int dim, const std::vector<Array<T>> &inputs);

INSTANTIATE(float)
INSTANTIATE(double)
INSTANTIATE(cfloat)
INSTANTIATE(cdouble)
INSTANTIATE(int)
INSTANTIATE(uint)
INSTANTIATE(intl)
INSTANTIATE(uintl)
INSTANTIATE(uchar)
INSTANTIATE(char)
INSTANTIATE(ushort)
INSTANTIATE(short)

#undef INSTANTIATE
}

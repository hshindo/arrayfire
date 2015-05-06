/*******************************************************
 * Copyright (c) 2014, ArrayFire
 * All rights reserved.
 *
 * This file is distributed under 3-clause BSD license.
 * The complete license agreement can be obtained at:
 * http://arrayfire.com/licenses/BSD-3-Clause
 ********************************************************/

#include <af/index.h>
#include <af/array.h>
#include <af/algorithm.h>
#include "error.hpp"
#include "common.hpp"

namespace af
{

array moddims(const array& in, const unsigned ndims, const dim_type * const dims)
{
    af_array out = 0;
    AF_THROW(af_moddims(&out, in.get(), ndims, dims));
    return array(out);
}

array moddims(const array& in, const dim4& dims)
{
    return af::moddims(in, dims.ndims(), dims.get());
}

array moddims(const array& in, dim_type d0, dim_type d1, dim_type d2, dim_type d3)
{
    dim_type dims[4] = {d0, d1, d2, d3};
    return af::moddims(in, 4, dims);
}

array flat(const array& in)
{
    af_array out = 0;
    AF_THROW(af_flat(&out, in.get()));
    return array(out);
}

array join(const int dim, const array& first, const array& second)
{
    af_array out = 0;
    AF_THROW(af_join(&out, dim, first.get(), second.get()));
    return array(out);
}

array join(const int dim, const array& first, const array& second, const array &third)
{
    af_array out = 0;
    af_array inputs[3] = {first.get(), second.get(), third.get()};
    AF_THROW(af_join_many(&out, dim, 3, inputs));
    return array(out);
}

array join(const int dim, const array& first, const array& second, const array &third, const array &fourth)
{
    af_array out = 0;
    af_array inputs[4] = {first.get(), second.get(), third.get(), fourth.get()};
    AF_THROW(af_join_many(&out, dim, 4, inputs));
    return array(out);
}

array tile(const array& in, const unsigned x, const unsigned y, const unsigned z, const unsigned w)
{
    af_array out = 0;
    AF_THROW(af_tile(&out, in.get(), x, y, z, w));
    return array(out);
}

array tile(const array& in, const af::dim4 &dims)
{
    af_array out = 0;
    AF_THROW(af_tile(&out, in.get(), dims[0], dims[1], dims[2], dims[3]));
    return array(out);
}

array reorder(const array& in, const unsigned x, const unsigned y, const unsigned z, const unsigned w)
{
    af_array out = 0;
    AF_THROW(af_reorder(&out, in.get(), x, y, z, w));
    return array(out);
}

array shift(const array& in, const int x, const int y, const int z, const int w)
{
    af_array out = 0;
    AF_THROW(af_shift(&out, in.get(), x, y, z, w));
    return array(out);
}

array flip(const array &in, const unsigned dim)
{
    af_array out = 0;
    AF_THROW(af_flip(&out, in.get(), dim));
    return array(out);
}

array lookup(const array &in, const array &idx, const int dim)
{
    af_array out = 0;
    AF_THROW(af_lookup(&out, in.get(), idx.get(), getFNSD(dim, in.dims())));
    return array(out);
}

indexer::indexer() {
    impl.mIndexer.seq = af_span;
    impl.mIsSeq = true;
    impl.isBatch = false;
}

indexer::indexer(const int idx) {
    impl.mIndexer.seq = af_make_seq(idx, idx, 1);
    impl.mIsSeq = true;
    impl.isBatch = false;
}

indexer::indexer(const af::seq& s0) {
    impl.mIndexer.seq = s0.s;
    impl.mIsSeq = true;
    impl.isBatch = s0.m_gfor;
}

indexer::indexer(const af_seq& s0) {
    impl.mIndexer.seq = s0;
    impl.mIsSeq = true;
    impl.isBatch = false;
}

indexer::indexer(const af::array& idx0) {
    array idx = idx0.isbool() ? where(idx0) : idx0;
    af_array arr = 0;
    AF_THROW(af_weak_copy(&arr, idx.get()));
    impl.mIndexer.arr = arr;

    impl.mIsSeq = false;
    impl.isBatch = false;
}


static bool operator==(const af_seq& lhs, const af_seq& rhs) {
    return lhs.begin == rhs.begin && lhs.end == rhs.end && lhs.step == rhs.step;
}

bool indexer::isspan() const
{
    return impl.mIsSeq == true && impl.mIndexer.seq == af_span;
}

const af_index_t& indexer::get() const
{
    return impl;
}

}

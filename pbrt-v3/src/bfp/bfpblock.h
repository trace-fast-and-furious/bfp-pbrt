#ifndef PBRT_BFP_BFPBLOCK_H
#define PBRT_BFP_BFPBLOCK_H

#include <stdint.h>

#include <vector>

#include "bfp/bfpnum.h"
#include "bfp/bfputility.cpp"

#define BFP_MANTISSA_LENGTH 23
#define FIXED_POINT_LENGTH 24
#define BFP_EXPONENT_LENGTH 8
#define BFP_BIAS (((uint16_t)1 << (BFP_EXPONENT_LENGTH - 1)) - (uint16_t)1)

namespace pbrt {
template <class F, class I>  // F: float/double, I: uint32_t/uint64_t
class BfpBlock {
  public:
    BfpBlock() : commonExp(0), blockSize(0){};
    BfpBlock(std::vector<F> x);

    void PrintBitwise();

  public:
    uint16_t commonExp;
    std::vector<uint16_t> sign;
    std::vector<I> mant;  // unsigned fixed point Q 1.23 or Q. 1.52 or custom

    uint32_t blockSize;
};

/*element-wise matrix arithmetic functions*/
template <class F, class I>
BfpBlock<F, I> Add1D(BfpBlock<F, I> a, BfpBlock<F, I> b);

template <class F, class I>
BfpBlock<F, I> Sub1D(BfpBlock<F, I> a, BfpBlock<F, I> b);

template <class F, class I>
BfpBlock<F, I> Mult1D(BfpBlock<F, I> a, BfpBlock<F, I> b);

template <class F, class I>
BfpBlock<F, I> Div1D(BfpBlock<F, I> a, BfpBlock<F, I> b);

template <class F, class I>
BfpBlock<F, I> Add2D(BfpBlock<F, I> a, BfpBlock<F, I> b);

template <class F, class I>
BfpBlock<F, I> Sub2D(BfpBlock<F, I> a, BfpBlock<F, I> b);

template <class F, class I>
BfpBlock<F, I> Mult2D(BfpBlock<F, I> a, BfpBlock<F, I> b);

template <class F, class I>
BfpBlock<F, I> Div2D(BfpBlock<F, I> a, BfpBlock<F, I> b);

/* scalar-matrix arithmetic functions*/
template <class F, class I>
BfpBlock<F, I> AddScalar1D(I aInt, BfpBlock<F, I> b);

template <class F, class I>
BfpBlock<F, I> SubScalar1D(I aInt, BfpBlock<F, I> b);

template <class F, class I>
BfpBlock<F, I> MultScalar1D(I aInt, BfpBlock<F, I> b);

template <class F, class I>
BfpBlock<F, I> DivScalar1D(I aInt, BfpBlock<F, I> b);

template <class F, class I>
BfpBlock<F, I> AddScalar2D(I aInt, BfpBlock<F, I> b);

template <class F, class I>
BfpBlock<F, I> SubScalar2D(I aInt, BfpBlock<F, I> b);

template <class F, class I>
BfpBlock<F, I> MultScalar2D(I aInt, BfpBlock<F, I> b);

template <class F, class I>
BfpBlock<F, I> DivScalar2D(I aInt, BfpBlock<F, I> b);

/* Matrix Multiplication */
template <class F, class I>
BfpBlock<F, I> MatrixMult(BfpBlock<F, I> a, BfpBlock<F, I> b);

/* compare functions */
template <class F, class I>
BfpNum<F, I> Max(BfpBlock<F, I> b);

template <class F, class I>
BfpNum<F, I> Min(BfpBlock<F, I> b);

template <class F, class I>
void Swap(BfpBlock<F, I> *a, BfpBlock<F, I> *b);
}  // namespace pbrt
#endif
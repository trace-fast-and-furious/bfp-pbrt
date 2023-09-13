#include "bfp/bfpnum.h"

#include <stdint.h>


namespace pbrt {
template <class F, class I>
BfpNum<F, I>::BfpNum(F f) {
    // check F type
    bool is_float = typeid(float) == typeid(F);
    mantBitsize = is_float ? (uint32_t)23 : (uint32_t)52;
    expBitsize = is_float ? (uint32_t)8 : (uint32_t)11;

    // cast to integer for bitwise operations
    I *temp = reinterpret_cast<I *>(&f);

    // extract mantissa bits using bitwise AND operation
    I mant_extract = ((I)1 << mantBitsize) - 1;
    mant = *temp & mant_extract;
    *temp >>= mantBitsize;

    // extract exponent bits using bitwise AND operation
    uint16_t exp_extract = ((uint16_t)1 << expBitsize) - 1;
    exp = (uint16_t)*temp & exp_extract;
    *temp >>= expBitsize;

    // add implicit one
    I implicit_one = (I)1 << mantBitsize;
    if (exp) mant ^= implicit_one;

    // extract sign bit using bitwise AND operation
    sign = (uint16_t)*temp & (uint16_t)1;
}

template <class F, class I>
BfpNum<F, I>::BfpNum(uint16_t s, uint16_t e, I m) {
    // check I type
    bool isFloat = typeid(I) == typeid(uint32_t);

    mantBitsize = isFloat ? (uint32_t)23 : (uint32_t)52;
    expBitsize = isFloat ? (uint32_t)8 : (uint32_t)11;
    sign = s;
    exp = e;

    // std::cout << BitString<uint16_t>(e, 8) << " " <<
    // BitStringWithSpace<uint32_t>(m, 32) << std::endl; normalization;
    I implicit1 = (I)1 << mantBitsize;
    mant = m;
    if (exp && mant) {
        while (!(mant & implicit1)) {
            mant <<= 1;
            exp -= 1;
        }
    }
}

template <class F, class I>
F BfpNum<F, I>::ToFloatingPoint() {
    I temp = 0;
    if (mant == 0) return 0;

    temp ^= (I)sign;
    temp <<= expBitsize;
    temp ^= (I)exp;
    temp <<= mantBitsize;
    temp ^= (mant &= ((1 << mantBitsize) - 1));  // remove implicit 1

    F *res = reinterpret_cast<F *>(&temp);
    return *res;
}

template <class F, class I>
void BfpNum<F, I>::PrintBitwise() {
    std::cout << "-----------BfpNum-----------" << std::endl;
    std::cout << BitString<uint16_t>(sign, 1) << " "
              << BitString<uint16_t>(exp, expBitsize) << " "
              << BitStringWithSpace<I>(mant, mantBitsize + 1) << std::endl;
}

template <class F, class I>
void BfpNum<F, I>::PrintValue() {
    std::cout << " [ " << ToFloatingPoint() << " ] " << std::endl;
}

}  // namespace pbrt
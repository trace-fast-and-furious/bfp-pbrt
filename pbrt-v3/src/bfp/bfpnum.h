#ifndef PBRT_BFP_BFPNUM_H
#define PBRT_BFP_BFPNUM_H

#include <stdint.h>

#include <iostream>
#include <string>
#include <vector>

#include "bfp/bfputility.cpp"

namespace pbrt {
template <class F, class I>  //<float, uint32_t> or <double, uint64_t>
class BfpNum {
  public:
    BfpNum(F f);
    BfpNum(uint16_t sign, uint16_t exp, I mant);
    F ToFloatingPoint();
    void PrintBitwise();
    void PrintValue();

  public:
    uint16_t sign;
    uint16_t exp;
    I mant;

    uint32_t mantBitsize;
    uint32_t expBitsize;
};
}  // namespace pbrt
#endif
#ifndef PBRT_BFP_BFPNUM_H
#define PBRT_BFP_BFPNUM_H

#include <stdint.h>

#include <iostream>
#include <string>
#include <vector>

#include "bfputility.h"

#define DOUBLE_MANTISSA_LENGTH 52
#define DOUBLE_EXPONENT_LENGTH 11

#define DOUBLE_MANT_EXTRACT_BITS 0x000FFFFFFFFFFFFF
#define DOUBLE_EXPONENT_EXTRACT_BITS 0x7FF0000000000000
#define DOUBLE_SIGN_EXTRACT_BITS 0x8000000000000000

#define DOUBLE_IMPLICIT_1 0x0010000000000000

namespace pbrt
{
    class BfpNum
    {
    public:
        BfpNum(double f);
        BfpNum(uint16_t sign, uint16_t exp, uint64_t mant);
        double ToFloatingPoint();
        void PrintBitwise();
        void PrintValue();

    public:
        uint16_t sign;
        uint16_t exp;
        uint64_t mant;
    };
}

#endif
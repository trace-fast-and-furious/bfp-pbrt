#ifndef PBRT_BFP_BFPNUM_H
#define PBRT_BFP_BFPNUM_H

#include "bfputility.h"


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
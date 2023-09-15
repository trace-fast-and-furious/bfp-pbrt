#ifndef PBRT_BFP_UTILITY_H
#define PBRT_BFP_UTILITY_H

#include <stdint.h>

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#define BFP_MANTISSA_LENGTH 23
#define FIXED_POINT_LENGTH 24
#define BFP_EXPONENT_LENGTH 8
#define BFP_BIAS (((uint16_t)1 << (BFP_EXPONENT_LENGTH - 1)) - (uint16_t)1)

namespace pbrt
{
    /* print functions */
    void printBit(uint64_t num, int len);
    void printBit(uint32_t num, int len);
    void printBit(uint16_t num, int len);
    std::string BitString(uint64_t num, uint32_t len);
    std::string BitString(uint32_t num, uint32_t len);
    std::string BitString(uint16_t num, uint32_t len);
    std::string BitStringWithSpace(uint64_t num, uint32_t len);
    /* rounding */
    int64_t RoundToNearestEven(int64_t num, int point);
}

#endif
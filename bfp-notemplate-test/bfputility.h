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
#define BFP_BIAS ((1 << (BFP_EXPONENT_LENGTH - 1)) - 1)

#define DOUBLE_BIAS 1023
#define DOUBLE_MANTISSA_LENGTH 52
#define DOUBLE_EXPONENT_LENGTH 11

#define DOUBLE_MANT_EXTRACT_BITS 0x000FFFFFFFFFFFFF
#define DOUBLE_EXPONENT_EXTRACT_BITS 0x7FF0000000000000
#define DOUBLE_SIGN_EXTRACT_BITS 0x8000000000000000

#define DOUBLE_IMPLICIT_1 0x0010000000000000

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
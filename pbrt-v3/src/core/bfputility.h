#ifndef PBRT_BFP_UTILITY_H
#define PBRT_BFP_UTILITY_H

#include <stdint.h>

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace pbrt {
/* print functions */
template <class T>
void printBit(T num, int len);

template <class T>
std::string BitString(T num, uint32_t len);

template <class T>
std::string BitStringWithSpace(T num, uint32_t len);
/* rounding */
int64_t RoundToNearestEven(int64_t num, int point);
}  // namespace pbrt
#endif
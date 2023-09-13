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
void printBit(T num, int len) {
    char out[100] = "";
    for (int i = 0; i < len; i++, num >>= 1) {
        if (num & 1)
            strcat(out, "1");
        else
            strcat(out, "0");
    }
    for (int i = len - 1; i >= 0; i--) {
        std::cout << out[i];
    }
    std::cout << " ";
}

template <class T>
std::string BitString(T num, uint32_t len) {
    std::string res = "";
    for (int i = 0; i < len; i++, num >>= 1) {
        if (num & 1)
            res = "1" + res;
        else
            res = "0" + res;
    }
    return res;
}

template <class T>
std::string BitStringWithSpace(T num, uint32_t len) {
    std::string res = "";
    for (int i = 0; i < len; i++, num >>= 1) {
        if (i % 4 == 0) res = " " + res;
        if (num & 1)
            res = "1" + res;
        else
            res = "0" + res;
    }
    return res;
}

/* rounding */
int64_t RoundToNearestEven(int64_t num, int point) {
    int64_t t = (int64_t)1 << point;

    bool lastBit = (bool)(num & t);
    t >>= 1;
    bool groundBit = (bool)(num & t);
    t >>= 1;
    bool roundBit = (bool)(num & t);
    t -= 1;
    bool stickyBits = (bool)(num & t);

    int64_t lsb = (int64_t)1 << point;
    if (groundBit) {
        if (!roundBit && !stickyBits) {  // round to even
            if (lastBit) num += lsb;
        } else
            num += lsb;  // round up
    }

    return num;
}
}  // namespace pbrt
#endif
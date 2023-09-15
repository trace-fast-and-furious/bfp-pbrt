#include "test.h"

#include "bfpnum.h"
#include "bfpblock.h"
#include "bfputility.h"
#include <stdint.h>

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <vector>

using namespace pbrt;

int main(void)
{
    std::vector<float> a{-1.0f, 1.5f, 1.75f, 1.00805664062f, -1.00805699825f};
    std::vector<float> b{-0.5f, -1.5f, 1.75f, 0.00805664062f, 1.00805699825f};
    std::vector<float> c{0.01171875f, 0.013671875f, 0.0146484375f, 0.01513671875f,
                         0.015380859375f}; // same exponent: 01111000

    BfpNum<float, uint32_t> test(a[0]);

    // printVectorBitwise(b);
    // testBfpBlock(a);
    // std::cout << "\n";
    // testBfpBlockAdd1D(a, b);
    // testBfpBlockSub1D(a, b);
    // testBfpBlockDiv1D(a, b);
    // testBfpBlockMult1D(a, b);
    // testAddScalar1D(1.5, b);
    // testSubScalar1D(1.5, b);
    // testMultScalar1D(1.012345, b);
    // testMax(b);
    // testMin(b);
    // testSwap(a, c);

    return 0;
}
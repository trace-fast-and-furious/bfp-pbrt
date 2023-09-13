#include <stdint.h>

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "bfp/bfpblock.h"
#include "bfp/bfpimpl.cpp"
#include "bfp/bfpnum.h"
#include "bfp/bfputility.cpp"

namespace pbrt {
std::vector<float> a{-1, 1.5, 1.75, 1.00805664062, -1.00805699825};
std::vector<float> b{-0.5, -1.5, 1.75, 0.00805664062, 1.00805699825};
std::vector<float> c{0.01171875, 0.013671875, 0.0146484375, 0.01513671875,
                     0.015380859375};  // same exponent: 01111000

void printVectorBitwise(std::vector<float> a) {
    for (int i = 0; i < a.size(); i++) {
        BfpNum<float, uint32_t> aBitwise(a[i]);
        aBitwise.PrintBitwise();
    }
}

void testBfpBlock(std::vector<float> a) {
    BfpBlock<float, uint32_t> aBlock(a);
    aBlock.PrintBitwise();
}

void testBfpBlockAdd1D(std::vector<float> a, std::vector<float> b) {
    BfpBlock<float, uint32_t> aBlock(a);
    BfpBlock<float, uint32_t> bBlock(b);
    BfpBlock<float, uint32_t> res = Add1D<float, uint32_t>(aBlock, bBlock);
    aBlock.PrintBitwise();
    bBlock.PrintBitwise();
    res.PrintBitwise();

    std::cout << "-------------compare-------------" << std::endl;
    for (int i = 0; i < a.size(); i++) {
        std::cout << i << ": " << a[i] + b[i] << std::endl;
    }
}

void testBfpBlockSub1D(std::vector<float> a, std::vector<float> b) {
    BfpBlock<float, uint32_t> aBlock(a);
    BfpBlock<float, uint32_t> bBlock(b);
    BfpBlock<float, uint32_t> res = Sub1D<float, uint32_t>(aBlock, bBlock);
    aBlock.PrintBitwise();
    bBlock.PrintBitwise();
    res.PrintBitwise();

    std::cout << "-------------compare-------------" << std::endl;
    for (int i = 0; i < a.size(); i++) {
        std::cout << i << ": " << a[i] - b[i] << std::endl;
    }
}

void testBfpBlockMult1D(std::vector<float> a, std::vector<float> b) {
    BfpBlock<float, uint32_t> aBlock(a);
    BfpBlock<float, uint32_t> bBlock(b);
    BfpBlock<float, uint32_t> res = Mult1D<float, uint32_t>(aBlock, bBlock);
    aBlock.PrintBitwise();
    bBlock.PrintBitwise();
    res.PrintBitwise();

    std::cout << "-------------compare-------------" << std::endl;
    for (int i = 0; i < a.size(); i++) {
        std::cout << i << ": " << a[i] * b[i] << std::endl;
    }
}

void testBfpBlockDiv1D(std::vector<float> a, std::vector<float> b) {
    BfpBlock<float, uint32_t> aBlock(a);
    BfpBlock<float, uint32_t> bBlock(b);
    BfpBlock<float, uint32_t> res = Div1D<float, uint32_t>(aBlock, bBlock);
    aBlock.PrintBitwise();
    bBlock.PrintBitwise();
    res.PrintBitwise();

    std::cout << "-------------compare-------------" << std::endl;
    for (int i = 0; i < a.size(); i++) {
        std::cout << i << ": " << a[i] / b[i] << std::endl;
    }
}

void testAddScalar1D(float aScalar, std::vector<float> b) {
    BfpBlock<float, uint32_t> bBlock(b);
    BfpBlock<float, uint32_t> res =
        AddScalar1D<float, uint32_t>(aScalar, bBlock);
    BfpNum<float, uint32_t> a(aScalar);
    a.PrintBitwise();
    bBlock.PrintBitwise();
    res.PrintBitwise();

    std::cout << "-------------compare-------------" << std::endl;
    for (int i = 0; i < b.size(); i++) {
        std::cout << i << ": " << aScalar + b[i] << std::endl;
    }
}

void testSubScalar1D(float aScalar, std::vector<float> b) {
    BfpBlock<float, uint32_t> bBlock(b);
    BfpBlock<float, uint32_t> res =
        SubScalar1D<float, uint32_t>(aScalar, bBlock);
    BfpNum<float, uint32_t> a(aScalar);
    a.PrintBitwise();
    bBlock.PrintBitwise();
    res.PrintBitwise();

    std::cout << "-------------compare-------------" << std::endl;
    for (int i = 0; i < b.size(); i++) {
        std::cout << i << ": " << aScalar - b[i] << std::endl;
    }
}

void testMultScalar1D(float aScalar, std::vector<float> b) {
    BfpBlock<float, uint32_t> bBlock(b);
    BfpBlock<float, uint32_t> res =
        MultScalar1D<float, uint32_t>(aScalar, bBlock);
    BfpNum<float, uint32_t> a(aScalar);
    a.PrintBitwise();
    std::cout << "\nx\n" << std::endl;
    bBlock.PrintBitwise();
    std::cout
        << "================================================================="
        << std::endl;
    res.PrintBitwise();

    std::cout << "-------------compare with actual fp value-------------"
              << std::endl;
    for (int i = 0; i < b.size(); i++) {
        std::cout << i << ": " << aScalar * b[i] << std::endl;
    }
}

void testMax(std::vector<float> a) {
    BfpBlock<float, uint32_t> aBlock(a);
    aBlock.PrintBitwise();
    std::cout << "\n\n======================<MAX VALUE>================"
              << std::endl;
    BfpNum<float, uint32_t> res = pbrt::Max(aBlock);
    res.PrintBitwise();
    res.PrintValue();
}

void testMin(std::vector<float> a) {
    BfpBlock<float, uint32_t> aBlock(a);
    aBlock.PrintBitwise();
    std::cout << "\n\n=====================<MIN VALUE>==================="
              << std::endl;
    BfpNum<float, uint32_t> res = pbrt::Min(aBlock);
    res.PrintBitwise();
    res.PrintValue();
}

void testSwap(std::vector<float> a, std::vector<float> b) {
    BfpBlock<float, uint32_t> aBlock(a);
    BfpBlock<float, uint32_t> bBlock(b);
    std::cout << "\n================<BEFORE SWAP>==============" << std::endl;
    aBlock.PrintBitwise();
    bBlock.PrintBitwise();
    Swap(&aBlock, &bBlock);
    std::cout << "\n\n================<AFTER SWAP>==============" << std::endl;
    aBlock.PrintBitwise();
    bBlock.PrintBitwise();
}

// int main(void)
// {
//     // printVectorBitwise(b);
//     // testBfpBlock(a);
//     // std::cout << "\n";
//     // testBfpBlockAdd1D(a, b);
//     // testBfpBlockSub1D(a, b);
//     // testBfpBlockDiv1D(a, b);
//     // testBfpBlockMult1D(a, b);
//     // testAddScalar1D(1.5, b);
//     // testSubScalar1D(1.5, b);
//     // testMultScalar1D(1.012345, b);
//     // testMax(b);
//     // testMin(b);
//     testSwap(a, c);

//     return 0;
// }
}  // namespace pbrt
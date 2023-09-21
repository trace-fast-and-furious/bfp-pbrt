#ifndef PBRT_BFP_BFPBLOCK_H
#define PBRT_BFP_BFPBLOCK_H

#include "bfpnum.h"

namespace pbrt
{
    class BfpBlock
    {
    public:
        BfpBlock() : commonExp(0), blockSize(0){};
        BfpBlock(std::vector<double> x);

        /*element-wise matrix arithmetic functions*/
        BfpBlock Add1D(BfpBlock b);
        BfpBlock Sub1D(BfpBlock b);
        BfpBlock Mult1D(BfpBlock b);
        BfpBlock Div1D(BfpBlock b);
        BfpBlock Add2D(BfpBlock b);
        BfpBlock Sub2D(BfpBlock b);
        BfpBlock Mult2D(BfpBlock b);
        BfpBlock Div2D(BfpBlock b);

        /* scalar-matrix arithmetic functions*/
        BfpBlock AddScalar1D(double scalar);
        BfpBlock SubScalar1D(double scalar, bool isScalarFirst);
        BfpBlock MultScalar1D(double scalar);
        BfpBlock DivScalar1D(double scalar, bool isScalarFirst);
        BfpBlock AddScalar2D(double scalar);
        BfpBlock SubScalar2D(double scalar);
        BfpBlock MultScalar2D(double scalar);
        BfpBlock DivScalar2D(double scalar);

        /* Matrix Multiplication */
        BfpBlock MatrixMult(BfpBlock b);

        /* compare functions */
        BfpNum Max();
        BfpNum Min();
        void Swap(BfpBlock *b);

        /* print functions */
        void PrintBitwise();

        std::vector<double> ToFloatingPoint();

    public:
        uint16_t commonExp;
        std::vector<uint16_t> sign;
        std::vector<uint64_t> mant; // unsigned fixed point Q 1.23 or Q. 1.52 or custom

        uint32_t blockSize;
    };

} // namespace pbrt
#endif
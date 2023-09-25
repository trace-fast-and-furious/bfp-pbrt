#ifndef PBRT_BFP_BFPBLOCK_H
#define PBRT_BFP_BFPBLOCK_H

#include "bfpnum.h"

namespace pbrt
{
    class BfpBlock
    {
    public:
        BfpBlock() : commonExp(0), m(0), n(0){};
        BfpBlock(std::vector<std::vector<double>> x);

        /*element-wise matrix arithmetic functions*/
        BfpBlock Add(BfpBlock b);
        BfpBlock Sub(BfpBlock b);
        BfpBlock Mult(BfpBlock b);
        // BfpBlock Div(BfpBlock b);

        /* scalar-matrix arithmetic functions*/
        BfpBlock AddScalar(double scalar);
        BfpBlock SubScalar(double scalar, bool isScalarFirst);
        BfpBlock MultScalar(double scalar);
        BfpBlock DivScalar(double scalar, bool isScalarFirst);

        /* Matrix Multiplication */
        BfpBlock MatrixMult(BfpBlock b);

        /* compare functions */
        BfpNum Max();
        BfpNum Min();
        void Swap(BfpBlock *b);

        /* print functions */
        void PrintBitwise();
        void PrintValue();

        std::vector<std::vector<double>> ToFloatingPoint();

    public:
        uint16_t commonExp;
        std::vector<std::vector<uint16_t>> sign;
        std::vector<std::vector<uint64_t>> mant; // unsigned fixed point Q 1.23 or Q. 1.52 or custom

        uint32_t m, n;
    };

} // namespace pbrt
#endif
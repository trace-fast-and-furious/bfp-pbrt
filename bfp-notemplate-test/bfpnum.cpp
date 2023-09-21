#include "bfpnum.h"

namespace pbrt
{
    BfpNum::BfpNum(double f)
    {
        // cast to integer for bitwise operations
        uint64_t *temp = reinterpret_cast<uint64_t *>(&f);

        // extract mantissa bits using bitwise AND operation
        mant = *temp & DOUBLE_MANT_EXTRACT_BITS;
        exp = (uint16_t)((*temp & DOUBLE_EXPONENT_EXTRACT_BITS) >> 52);
        sign = (uint16_t)((*temp & DOUBLE_SIGN_EXTRACT_BITS) >> 63);

        // add implicit one
        if (exp)
            mant ^= DOUBLE_IMPLICIT_1;
    }

    BfpNum::BfpNum(uint16_t s, uint16_t e, uint64_t m)
    {
        sign = s;
        exp = e - BFP_BIAS + DOUBLE_BIAS;
        mant = m << (DOUBLE_MANTISSA_LENGTH - BFP_MANTISSA_LENGTH); //implicit 1 included!
        if (exp && mant)
        {
            while (!(mant & DOUBLE_IMPLICIT_1))
            {
                mant <<= 1;
                exp -= 1;
            }
        }
    }

    double BfpNum::ToFloatingPoint()
    {
        uint64_t temp = 0;
        if (mant == 0)
            return 0;

        temp ^= (uint64_t)sign;
        temp <<= 11;
        temp ^= (uint64_t)exp;
        temp <<= 52;
        temp ^= (mant ^ DOUBLE_IMPLICIT_1); // remove implicit 1

        double *res = reinterpret_cast<double *>(&temp);
        return *res;
    }

    void BfpNum::PrintBitwise()
    {
        std::cout << "-----------BfpNum-----------" << std::endl;
        std::cout << BitString(sign, 1) << " " << BitString(exp, 11) << " " << BitStringWithSpace(mant, 53) << std::endl;
    }

    void BfpNum::PrintValue()
    {
        std::cout << " [ " << ToFloatingPoint() << " ] " << std::endl;
    }
}
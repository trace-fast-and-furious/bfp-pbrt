#include "bfpblock.h"

namespace pbrt
{
    /* block formatting */
    BfpBlock::BfpBlock(std::vector<std::vector<double>> x)
    {
        // initialize some member variables
        m = x.size();
        n = x[0].size();
        sign.resize(m, std::vector<uint16_t>(n, 0));
        mant.resize(m, std::vector<uint64_t>(n, 0));

        // check customized mantissa/exponent length is possible
        if (BFP_MANTISSA_LENGTH > 52)
            throw std::invalid_argument(
                "error: BFP_MANTISSA_LENGTH is too long for double!");
        else if (BFP_EXPONENT_LENGTH > 11)
            throw std::invalid_argument(
                "error: BFP_EXPONENT_LENGTH is too long for double!");

        // block formatting
        std::vector<std::vector<uint16_t>> exps(m, std::vector<uint16_t>(n, 0));

        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            { // customize mantissa/exponent length
                BfpNum *temp = new BfpNum(x[i][j]);
                exps[i][j] = temp->exp - DOUBLE_BIAS + BFP_BIAS;
                mant[i][j] = temp->mant >> (DOUBLE_MANTISSA_LENGTH - BFP_MANTISSA_LENGTH); // implicit 1 already added in BfpNum
                sign[i][j] = temp->sign;
                delete temp;
            }
        }

        // find and save common exponent
        uint16_t max_exp = 0;
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                max_exp = std::max(exps[i][j], max_exp);
            }
        }
        commonExp = max_exp;

        // align mantissas
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                mant[i][j] >>= (max_exp - exps[i][j]);
            }
        }
    }

    /* matrix element-wise arithmetic functions */
    // this block: a, other block: b
    BfpBlock BfpBlock::Add(BfpBlock b)
    {
        uint16_t carry = 0;
        BfpBlock res;
        res.sign.resize(m, std::vector<uint16_t>(n, 0));
        res.mant.resize(m, std::vector<uint64_t>(n, 0));

        // check if both blocks have same size
        if (m != b.m || n != b.n)
            throw std::invalid_argument(
                "error[Add]: two blocks are not the same size");

        // set blockSize
        res.m = m;
        res.n = n;

        // decide common exponent
        bool flag = commonExp >= b.commonExp ? 1 : 0;
        res.commonExp = flag ? commonExp : b.commonExp;
        int diff = std::abs(commonExp - b.commonExp);

        // shift mantissas in long long so that there is no data loss during shifting
        int64_t tempImplicit1 = (int64_t)1 << 61;
        int64_t tempShiftNum = 61 - BFP_MANTISSA_LENGTH; // sign + carry + implicit1 + mantissa + tempShiftNum = 64

        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                int64_t tempRes = (int64_t)0;
                int64_t tempA = mant[i][j] << tempShiftNum;
                int64_t tempB = b.mant[i][j] << tempShiftNum;

                // align mantissas
                if (flag) // if this block(a)'s common exponent is bigger
                    tempB >>= diff;
                else
                    tempA >>= diff;

                // add mantissa
                // 1. conversion to 2's complement
                tempA = sign[i][j] ? ~tempA + 1 : tempA;
                tempB = b.sign[i][j] ? ~tempB + 1 : tempB;

                // 2. add mantissas
                tempRes = tempA + tempB;

                // 3. convert to signed magnitude if negative
                if (tempRes & 0x8000000000000000)
                {
                    tempRes = ~tempRes + 1;
                    res.sign[i][j] = (uint16_t)1;
                }
                else
                {
                    res.sign[i][j] = (uint16_t)0;
                }

                // 4. rounding to nearest even
                tempRes = RoundToNearestEven(tempRes, tempShiftNum);

                // 5. check carry
                carry = std::max(carry, (uint16_t)(tempRes >> (int64_t)62));

                // 6. store result
                res.mant[i][j] = (uint64_t)(tempRes >> tempShiftNum);
            }
        }

        if (carry)
        {
            res.commonExp += carry;
            for (int i = 0; i < m; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    res.mant[i][j] >>= carry;
                }
            }
        }

        return res;
    }

    BfpBlock BfpBlock::Sub(BfpBlock b)
    {
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                b.sign[i][j] ^= (uint16_t)1;
            }
        }

        BfpBlock res = Add(b);
        return res;
    }

    BfpBlock BfpBlock::Mult(BfpBlock b)
    {
        uint16_t carry = 0;
        BfpBlock res;
        res.sign.resize(m, std::vector<uint16_t>(n, 0));
        res.mant.resize(m, std::vector<uint64_t>(n, 0));

        // check if both blocks have same size
        if (m != b.m || n != b.n)
            throw std::invalid_argument(
                "error[Mult]: two blocks are not the same size");

        // set blockSize
        res.m = m;
        res.n = n;

        // decide common exponent
        res.commonExp = commonExp + b.commonExp - BFP_BIAS;

        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                // 1. multiply
                uint64_t tempRes = mant[i][j] * b.mant[i][j];

                // 2. set sign
                res.sign[i][j] = sign[i][j] ^ b.sign[i][j];

                // 3. rounding to nearest even
                tempRes = RoundToNearestEven(tempRes, BFP_MANTISSA_LENGTH);

                // 4. check carry
                carry = std::max(carry, (uint16_t)(tempRes >> (uint64_t)(BFP_MANTISSA_LENGTH + BFP_MANTISSA_LENGTH + 1)));

                // 5. store result
                res.mant[i][j] = tempRes >> BFP_MANTISSA_LENGTH;
            }
        }

        if (carry)
        {
            res.commonExp += carry;
            for (int i = 0; i < m; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    res.mant[i][j] >>= carry;
                }
            }
        }

        return res;
    }

    // BfpBlock BfpBlock::Div1D(BfpBlock b)
    // {
    //     uint16_t carry = 0;
    //     BfpBlock res;

    //     // check if both blocks have same size
    //     if (blockSize != b.blockSize)
    //         throw std::invalid_argument(
    //             "error[Div1D]: two blocks are not the same size");

    //     // set blockSize
    //     res.blockSize = blockSize;

    //     // decide common exponent
    //     res.commonExp = commonExp - b.commonExp + BFP_BIAS;

    //     // save mantissas in long long so that there is no data loss during shifting
    //     uint64_t tempShiftNum = 61 - BFP_MANTISSA_LENGTH; // sign + carry + implicit1 + mantissa + tempShiftNum = 64
    //     uint64_t tempImplicit1 = (uint64_t)1 << 61;
    //     for (int i = 0; i < blockSize; i++)
    //     {
    //         // 1. divide
    //         uint64_t tempA = mant[i] << tempShiftNum;
    //         uint64_t tempB = b.mant[i];
    //         uint64_t tempRes = tempA / tempB;

    //         // std::cout << "    " << BitStringWithSpace<int64_t>(tempA, 64)
    //         //           << std::endl;
    //         // std::cout << "/   " << BitStringWithSpace<int64_t>(tempB, 64)
    //         //           << std::endl;
    //         // std::cout << "------------------" << std::endl;
    //         // std::cout << "    " << BitStringWithSpace<int64_t>(tempRes, 64)
    //         //           << std::endl
    //         //           << std::endl;

    //         // 2. set sign
    //         res.sign.push_back(sign[i] ^ b.sign[i]);

    //         // 3. rounding to nearest even
    //         tempRes = RoundToNearestEven(tempRes, BFP_MANTISSA_LENGTH);

    //         // 4. check carry
    //         carry = std::max(carry, (uint16_t)(tempRes >> (uint64_t)(61 - BFP_MANTISSA_LENGTH)));

    //         // std::cout << carry << std::endl;

    //         // 5. store result
    //         res.mant.push_back(tempRes >> (uint64_t)(61 - BFP_MANTISSA_LENGTH - BFP_MANTISSA_LENGTH));
    //     }

    //     if (carry)
    //     {
    //         res.commonExp += carry;
    //         for (int i = 0; i < res.blockSize; i++)
    //         {
    //             res.mant[i] >>= (uint64_t)carry;
    //         }
    //     }

    //     return res;
    // }

    /* scalar-matrix arithmetic functions*/
    BfpBlock BfpBlock::AddScalar(double scalar)
    {
        uint16_t carry = 0;
        BfpBlock res;
        res.sign.resize(m, std::vector<uint16_t>(n, 0));
        res.mant.resize(m, std::vector<uint64_t>(n, 0));

        // set blockSize
        res.m = m;
        res.n = n;

        // get a's sign, exponent, mantissa bits
        BfpNum s(scalar);

        // adjust exponent, mantissa into bfp precision
        s.exp = s.exp - DOUBLE_BIAS + BFP_BIAS;
        s.mant >>= (DOUBLE_MANTISSA_LENGTH - BFP_MANTISSA_LENGTH);

        // decide common exponent
        bool flag = s.exp >= commonExp ? 1 : 0;
        res.commonExp = flag ? s.exp : commonExp;
        int diff = std::abs(s.exp - commonExp);

        // shift mantissas in long long so that there is no data loss during shifting
        uint64_t tempImplicit1 = (uint64_t)1 << 61;
        uint64_t tempShiftNum = 61 - BFP_MANTISSA_LENGTH; // sign + carry + implicit1 + mantissa + tempShiftNum = 64

        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                uint64_t tempRes = (uint64_t)0;
                uint64_t tempS = s.mant << tempShiftNum;
                uint64_t tempB = mant[i][j] << tempShiftNum;

                // align mantissas
                if (flag) // if a's common exponent is bigger
                    tempB >>= diff;
                else
                    tempS >>= diff;

                // add mantissa
                // 1. conversion to 2's complement
                tempS = s.sign ? ~tempS + 1 : tempS;
                tempB = sign[i][j] ? ~tempB + 1 : tempB;

                // 2. add mantissas
                tempRes = tempS + tempB;

                // 3. convert to signed magnitude if negative
                if (tempRes & 0x8000000000000000)
                {
                    tempRes = ~tempRes + 1;
                    res.sign[i][j] = (uint16_t)1;
                }
                else
                {
                    res.sign[i][j] = (uint16_t)0;
                }

                // 4. rounding to nearest even
                tempRes = RoundToNearestEven(tempRes, tempShiftNum);

                // 5. check carry
                carry = std::max(carry, (uint16_t)(tempRes >> (uint64_t)62));
                // std::cout << carry << std::endl;

                // 6. store result
                res.mant[i][j] = tempRes >> tempShiftNum;
            }
        }

        if (carry)
        {
            res.commonExp += carry;
            for (int i = 0; i < m; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    res.mant[i][j] >>= carry;
                }
            }
        }

        return res;
    }

    BfpBlock BfpBlock::SubScalar(double scalar, bool isScalarFirst)
    {
        BfpBlock res;
        if (isScalarFirst) // scalar - bfp
        {
            for (int i = 0; i < m; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    sign[i][j] ^= (uint16_t)1;
                }
            }

            res = AddScalar(scalar);
        }
        else // bfp - scalar
        {
            res = AddScalar(-scalar);
        }
        return res;
    }

    BfpBlock BfpBlock::MultScalar(double scalar)
    {
        uint16_t carry = false;
        BfpBlock res;
        res.sign.resize(m, std::vector<uint16_t>(n, 0));
        res.mant.resize(m, std::vector<uint64_t>(n, 0));

        // get a's sign, exponent, mantissa bits
        BfpNum s(scalar);

        // adjust exponent, mantissa into bfp precision
        s.exp = s.exp - DOUBLE_BIAS + BFP_BIAS;
        s.mant >>= (DOUBLE_MANTISSA_LENGTH - BFP_MANTISSA_LENGTH);

        // set blockSize
        res.m = m;
        res.n = n;

        // decide common exponent
        res.commonExp = s.exp + commonExp - BFP_BIAS;

        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                // 1. multiply
                uint64_t tempRes = s.mant * mant[i][j];

                // 2. set sign
                res.sign[i][j] = s.sign ^ sign[i][j];

                // 3. rounding to nearest even
                tempRes = RoundToNearestEven(tempRes, BFP_MANTISSA_LENGTH);

                // 4. check carry
                carry = std::max(carry, (uint16_t)(tempRes >> (uint64_t)(BFP_MANTISSA_LENGTH + BFP_MANTISSA_LENGTH + 1)));

                // 5. store result
                res.mant[i][j] = tempRes >> BFP_MANTISSA_LENGTH;
            }
        }

        if (carry)
        {
            res.commonExp += carry;
            for (int i = 0; i < m; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    res.mant[i][j] >>= carry;
                }
            }
        }

        return res;
    }

    /* Matrix Multiplication */
    BfpBlock BfpBlock::MatrixMult(BfpBlock b)
    {
        uint16_t carry = 0;
        BfpBlock res;
        res.sign.resize(m, std::vector<uint16_t>(b.n, 0));
        res.mant.resize(m, std::vector<uint64_t>(b.n, 0));

        // check if matrix multiplication is possible
        if (n != b.m)
            throw std::invalid_argument(
                "error[MatrixMult]: blocks' size is unfit for Matrix Multiplication");

        // set blockSize
        res.m = m;
        res.n = b.n;

        // decide common exponent
        res.commonExp = commonExp + b.commonExp - BFP_BIAS;

        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                // 1. multiply
                uint64_t tempRes = mant[i][j] * b.mant[i][j];

                // 2. set sign
                res.sign[i][j] = sign[i][j] ^ b.sign[i][j];

                // 3. rounding to nearest even
                tempRes = RoundToNearestEven(tempRes, BFP_MANTISSA_LENGTH);

                // 4. check carry
                carry = std::max(carry, (uint16_t)(tempRes >> (uint64_t)(BFP_MANTISSA_LENGTH + BFP_MANTISSA_LENGTH + 1)));

                // 5. store result
                res.mant[i][j] = tempRes >> BFP_MANTISSA_LENGTH;
            }
        }

        if (carry)
        {
            res.commonExp += carry;
            for (int i = 0; i < m; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    res.mant[i][j] >>= carry;
                }
            }
        }

        return res;
    }

    /* compare functions */
    BfpNum BfpBlock::Max()
    {
        int64_t maxMant = 0x8000000000000000;

        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                int64_t signedMant = sign[i][j] ? ~mant[i][j] + (int64_t)1 : mant[i][j];
                maxMant = signedMant > maxMant ? signedMant : maxMant;
            }
        }

        if (maxMant & (int64_t)1 << 63)
            return BfpNum((uint16_t)1, commonExp, (uint64_t)(~maxMant + (int64_t)1));
        else
            return BfpNum((uint16_t)0, commonExp, maxMant);
    }

    BfpNum BfpBlock::Min()
    {
        int64_t minMant = 0x7FFFFFFFFFFFFFFF;

        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                int64_t signedMant = sign[i][j] ? ~(int64_t)mant[i][j] + (int64_t)1 : (int64_t)mant[i][j];
                minMant = signedMant < minMant ? signedMant : minMant;
            }
        }

        if (minMant & (int64_t)1 << 63)
            return BfpNum((uint16_t)1, commonExp, (uint64_t)(~minMant + (int64_t)1));
        else
            return BfpNum((uint16_t)0, commonExp, (uint64_t)minMant);
    }

    void BfpBlock::Swap(BfpBlock *b)
    {
        // check if both blocks have same size
        // check if both blocks have same size
        if (m != b->m || n != b->n)
            throw std::invalid_argument(
                "error[Swap]: two blocks are not the same size");

        // alter commonExp and mantissas before comparison
        if (commonExp > b->commonExp)
        {
            // align mantissas so that mantissas corresponds to the bigger
            // common exponent
            for (int i = 0; i < m; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    b->mant[i][j] >>= (commonExp - b->commonExp);
                }
            }

            // set smaller commonExp to the bigger commonExp
            b->commonExp = commonExp;
        }
        else if (b->commonExp > commonExp)
        {
            // align mantissas so that mantissas corresponds to the bigger
            // common exponent
            for (int i = 0; i < m; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    mant[i][j] >>= (b->commonExp - commonExp);
                }
            }

            // set smaller commonExp to the bigger commonExp
            commonExp = b->commonExp;
        }

        // compare and swap (a: max block, b: min block)
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                int64_t signedAMant = sign[i][j] ? ~(int64_t)mant[i][j] + (int64_t)1 : (int64_t)mant[i][j];
                int64_t signedBMant = b->sign[i][j] ? ~(int64_t)b->mant[i][j] + (int64_t)1 : (int64_t)b->mant[i][j];

                if (signedBMant > signedAMant) // compare
                {
                    uint64_t mantTemp = b->mant[i][j]; // swap
                    b->mant[i][j] = mant[i][j];
                    mant[i][j] = mantTemp;

                    uint16_t signTemp = b->sign[i][j];
                    b->sign[i][j] = sign[i][j];
                    sign[i][j] = signTemp;
                }
            }
        }
    }

    /* print functions */
    void BfpBlock::PrintBitwise()
    {
        std::cout << "--------------BFP block-------------" << std::endl;
        std::cout << "MxN:\t" << m << "x" << n << std::endl;
        std::cout << "commonExp: " << BitString(commonExp, BFP_EXPONENT_LENGTH) << std::endl;
        for (int i = 0; i < m; i++)
        {
            std::cout << i << std::endl;
            for (int j = 0; j < n; j++)
            {
                BfpNum b(sign[i][j], commonExp, mant[i][j]);
                std::cout
                    << "   | " << j << ":   " << sign[i][j] << "   " << BitStringWithSpace(mant[i][j], FIXED_POINT_LENGTH);
                std::cout << "(" << b.ToFloatingPoint() << ")" << std::endl;
            }
        }
        std::cout << std::endl;
    }

    void BfpBlock::PrintValue()
    {
        std::cout << "--------------BFP block-------------" << std::endl;
        std::cout << "MxN:\t" << m << "x" << n << std::endl;
        std::cout << "commonExp: " << BitString(commonExp, BFP_EXPONENT_LENGTH) << std::endl;
        for (int i = 0; i < m; i++)
        {
            std::cout << i << std::endl;
            for (int j = 0; j < n; j++)
            {
                BfpNum b(sign[i][j], commonExp, mant[i][j]);
                std::cout
                    << "   | " << b.ToFloatingPoint() << std::endl;
            }
        }
        std::cout << std::endl;
    }

    std::vector<std::vector<double>> BfpBlock::ToFloatingPoint()
    {
        std::vector<std::vector<double>> res;
        for (int i = 0; i < m; i++)
        {
            std::vector<double> row;
            for (int j = 0; j < n; j++)
            {
                BfpNum b(sign[i][j], commonExp, mant[i][j]);
                row.push_back(b.ToFloatingPoint());
            }
            res.push_back(row);
        }
        return res;
    }

} // namespace pbrt
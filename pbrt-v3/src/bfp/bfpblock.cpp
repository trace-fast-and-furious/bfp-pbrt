#include "bfp/bfpblock.h"

#include <stdint.h>

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "bfp/bfpnum.h"
#include "bfp/bfputility.h"

namespace pbrt {
/* block formatting */
template <class F, class I>
BfpBlock<F, I>::BfpBlock(std::vector<F> x) {
    // initialize some member variables
    blockSize = (uint32_t)x.size();

    // check customized mantissa/exponent length is possible
    bool isFloat = typeid(float) == typeid(F);
    uint32_t fMantBitsize = isFloat ? 23 : 52;
    uint32_t fExpBitsize = isFloat ? 8 : 11;

    if (isFloat) {
        if (BFP_MANTISSA_LENGTH > fMantBitsize)
            throw std::invalid_argument(
                "error: BFP_MANTISSA_LENGTH is too long for float!");
        else if (BFP_EXPONENT_LENGTH > fExpBitsize)
            throw std::invalid_argument(
                "error: BFP_EXPONENT_LENGTH is too long for float!");
    } else {
        if (BFP_MANTISSA_LENGTH > fMantBitsize)
            throw std::invalid_argument(
                "error: BFP_MANTISSA_LENGTH is too long for double!");
        else if (BFP_EXPONENT_LENGTH > fExpBitsize)
            throw std::invalid_argument(
                "error: BFP_EXPONENT_LENGTH is too long for double!");
    }

    // block formatting
    std::vector<uint16_t> exps;

    for (int i = 0; i < x.size(); i++) {
        // customize mantissa/exponent length
        BfpNum<F, I> temp(x[i]);
        exps.push_back(temp.exp >> (fExpBitsize - BFP_EXPONENT_LENGTH));
        mant.push_back(
            temp.mant >>
            (fMantBitsize -
             BFP_MANTISSA_LENGTH));  // implicit 1 already added in BfpNum
        sign.push_back(temp.sign);
    }

    // find and save common exponent
    uint16_t max_exp = 0;
    for (int i = 0; i < x.size(); i++) {
        if (exps[i] > max_exp) max_exp = exps[i];
    }
    commonExp = max_exp;

    // align mantissas
    for (int i = 0; i < x.size(); i++) mant[i] >>= (max_exp - exps[i]);
}

/* matrix element-wise arithmetic functions */
template <class F, class I>
BfpBlock<F, I> Add1D(BfpBlock<F, I> a, BfpBlock<F, I> b) {
    uint16_t carry = false;

    // check if both blocks have same size
    if (a.blockSize != b.blockSize)
        throw std::invalid_argument(
            "error[Add1D]: two blocks are not the same size");

    // set blockSize
    BfpBlock<F, I> res;
    res.blockSize = a.blockSize;

    // decide common exponent
    bool flag = a.commonExp >= b.commonExp ? 1 : 0;
    res.commonExp = flag ? a.commonExp : b.commonExp;
    int diff = std::abs(a.commonExp - b.commonExp);

    for (int i = 0; i < a.blockSize; i++) {
        // save mantissas in long long so that there is no data loss during
        // shifting
        int tempShiftNum =
            61 - BFP_MANTISSA_LENGTH;  // sign + carry + implicit1 + mantissa +
                                       // tempShiftNum = 64
        int64_t implicit1 = (int64_t)1 << 61;
        int64_t tempRes = (int64_t)0;
        int64_t tempA = (int64_t)a.mant[i] << tempShiftNum;
        int64_t tempB = (int64_t)b.mant[i] << tempShiftNum;

        // align mantissas
        if (flag)  // if a's common exponent is bigger
            tempB >>= diff;
        else
            tempA >>= diff;

        // add mantissa
        // 1. conversion to 2's complement
        tempA = a.sign[i] ? ~tempA + 1 : tempA;
        tempB = b.sign[i] ? ~tempB + 1 : tempB;

        // 2. add mantissas
        tempRes = tempA + tempB;

        // std::cout << i << std::endl;
        // std::cout << "   " << BitString<uint64_t>(tempA, 64) << "\t" <<
        // BitString<uint32_t>(a.mant[i], 24) << std::endl; std::cout << "+  "
        // << BitString<uint64_t>(tempB, 64) << "\t" <<
        // BitString<uint32_t>(b.mant[i], 24) << std::endl; std::cout <<
        // "----------------------------" << std::endl; std::cout << "   " <<
        // BitString<uint64_t>(tempRes, 64) << std::endl
        //           << std::endl;

        // 3. convert to signed magnitude if negative
        if (tempRes & 0x8000000000000000) {
            tempRes = ~tempRes + 1;
            res.sign.push_back((uint16_t)1);
        } else {
            res.sign.push_back((uint16_t)0);
        }

        // 4. rounding to nearest even
        tempRes = RoundToNearestEven(tempRes, tempShiftNum);

        // 5. check carry
        carry = std::max(carry, (uint16_t)(tempRes >> (int64_t)62));

        // 6. store result
        res.mant.push_back((I)(tempRes >> tempShiftNum));
    }

    if (carry) {
        res.commonExp += carry;
        for (int i = 0; i < res.blockSize; i++) {
            res.mant[i] >>= carry;
        }
    }

    return res;
}

template <class F, class I>
BfpBlock<F, I> Sub1D(BfpBlock<F, I> a, BfpBlock<F, I> b) {
    for (int i = 0; i < b.blockSize; i++) b.sign[i] ^= (uint16_t)1;

    BfpBlock<F, I> res = Add1D<F, I>(a, b);
    return res;
}

template <class F, class I>
BfpBlock<F, I> Mult1D(BfpBlock<F, I> a, BfpBlock<F, I> b) {
    uint16_t carry = false;

    // check if both blocks have same size
    if (a.blockSize != b.blockSize)
        throw std::invalid_argument(
            "error[Add1D]: two blocks are not the same size");

    // set blockSize
    BfpBlock<F, I> res;
    res.blockSize = a.blockSize;

    // decide common exponent
    res.commonExp = a.commonExp + b.commonExp - BFP_BIAS;

    for (int i = 0; i < a.blockSize; i++) {
        // 1. multiply
        int64_t tempRes = (int64_t)a.mant[i] * (int64_t)b.mant[i];

        // 2. set sign
        res.sign.push_back(a.sign[i] ^ b.sign[i]);

        // 3. rounding to nearest even
        tempRes = RoundToNearestEven(tempRes, BFP_MANTISSA_LENGTH);

        // 4. check carry
        carry = std::max(
            carry, (uint16_t)(tempRes >> (int64_t)(BFP_MANTISSA_LENGTH +
                                                   BFP_MANTISSA_LENGTH + 1)));

        // 5. store result
        res.mant.push_back((I)(tempRes >> BFP_MANTISSA_LENGTH));
    }

    if (carry) {
        res.commonExp += carry;
        for (int i = 0; i < res.blockSize; i++) {
            res.mant[i] >>= carry;
        }
    }

    return res;
}

template <class F, class I>
BfpBlock<F, I> Div1D(BfpBlock<F, I> a, BfpBlock<F, I> b) {
    uint16_t carry = 0;

    // check if both blocks have same size
    if (a.blockSize != b.blockSize)
        throw std::invalid_argument(
            "error[Add1D]: two blocks are not the same size");

    // set blockSize
    BfpBlock<F, I> res;
    res.blockSize = a.blockSize;

    // decide common exponent
    res.commonExp = a.commonExp - b.commonExp + BFP_BIAS;

    for (int i = 0; i < a.blockSize; i++) {
        // 1. divide
        // save mantissas in long long so that there is no data loss during
        // shifting
        int tempShiftNum =
            61 - BFP_MANTISSA_LENGTH;  // sign + carry + implicit1 + mantissa +
                                       // tempShiftNum = 64
        int64_t implicit1 = (int64_t)1 << 61;
        int64_t tempA = (int64_t)a.mant[i] << tempShiftNum;
        int64_t tempB = (int64_t)b.mant[i];
        int64_t tempRes = tempA / tempB;

        std::cout << "    " << BitStringWithSpace<int64_t>(tempA, 64)
                  << std::endl;
        std::cout << "/   " << BitStringWithSpace<int64_t>(tempB, 64)
                  << std::endl;
        std::cout << "------------------" << std::endl;
        std::cout << "    " << BitStringWithSpace<int64_t>(tempRes, 64)
                  << std::endl
                  << std::endl;

        // 2. set sign
        res.sign.push_back(a.sign[i] ^ b.sign[i]);

        // 3. rounding to nearest even
        tempRes = RoundToNearestEven(tempRes, BFP_MANTISSA_LENGTH);

        // 4. check carry
        carry = std::max(
            carry, (uint16_t)(tempRes >> (int64_t)(61 - BFP_MANTISSA_LENGTH)));

        std::cout << carry << std::endl;
        // 5. store result
        res.mant.push_back((
            I)(tempRes >> (I)(61 - BFP_MANTISSA_LENGTH - BFP_MANTISSA_LENGTH)));
    }

    if (carry) {
        res.commonExp += carry;
        for (int i = 0; i < res.blockSize; i++) {
            res.mant[i] >>= (I)carry;
        }
    }

    return res;
}

template <class F, class I>
BfpBlock<F, I> Add2D(BfpBlock<F, I> a, BfpBlock<F, I> b) {
    uint16_t carry = false;

    // check if both blocks have same size
    if (a.blockSize != b.blockSize)
        throw std::invalid_argument(
            "error[Add1D]: two blocks are not the same size");

    // set blockSize
    BfpBlock<F, I> res;
    res.blockSize = a.blockSize;

    // decide common exponent
    bool flag = a.commonExp >= b.commonExp ? 1 : 0;
    res.commonExp = flag ? a.commonExp : b.commonExp;
    int diff = std::abs(a.commonExp - b.commonExp);

    for (int i = 0; i < a.blockSize; i++) {
        // save mantissas in long long so that there is no data loss during
        // shifting
        int tempShiftNum =
            61 - BFP_MANTISSA_LENGTH;  // sign + carry + implicit1 + mantissa +
                                       // tempShiftNum = 64
        int64_t implicit1 = (int64_t)1 << 61;
        int64_t tempRes = (int64_t)0;
        int64_t tempA = (int64_t)a.mant[i] << tempShiftNum;
        int64_t tempB = (int64_t)b.mant[i] << tempShiftNum;

        // align mantissas
        if (flag)  // if a's common exponent is bigger
            tempB >>= diff;
        else
            tempA >>= diff;

        // add mantissa
        // 1. conversion to 2's complement
        tempA = a.sign[i] ? ~tempA + 1 : tempA;
        tempB = b.sign[i] ? ~tempB + 1 : tempB;

        // 2. add mantissas
        tempRes = tempA + tempB;

        // std::cout << i << std::endl;
        // std::cout << "   " << BitString<uint64_t>(tempA, 64) << "\t" <<
        // BitString<uint32_t>(a.mant[i], 24) << std::endl; std::cout << "+  "
        // << BitString<uint64_t>(tempB, 64) << "\t" <<
        // BitString<uint32_t>(b.mant[i], 24) << std::endl; std::cout <<
        // "----------------------------" << std::endl; std::cout << "   " <<
        // BitString<uint64_t>(tempRes, 64) << std::endl
        //           << std::endl;

        // 3. convert to signed magnitude if negative
        if (tempRes & 0x8000000000000000) {
            tempRes = ~tempRes + 1;
            res.sign.push_back((uint16_t)1);
        } else {
            res.sign.push_back((uint16_t)0);
        }

        // 4. rounding to nearest even
        tempRes = RoundToNearestEven(tempRes, tempShiftNum);

        // 5. check carry
        carry = std::max(carry, (uint16_t)(tempRes >> (int64_t)62));

        // 6. store result
        res.mant.push_back((I)(tempRes >> tempShiftNum));
    }

    if (carry) {
        res.commonExp += carry;
        for (int i = 0; i < res.blockSize; i++) {
            res.mant[i] >>= carry;
        }
    }

    return res;
}

template <class F, class I>
BfpBlock<F, I> Sub2D(BfpBlock<F, I> a, BfpBlock<F, I> b) {
    for (int i = 0; i < b.blockSize; i++) b.sign[i] ^= (uint16_t)1;

    BfpBlock<F, I> res = Add1D<F, I>(a, b);
    return res;
}

template <class F, class I>
BfpBlock<F, I> Mult2D(BfpBlock<F, I> a, BfpBlock<F, I> b) {
    uint16_t carry = false;

    // check if both blocks have same size
    if (a.blockSize != b.blockSize)
        throw std::invalid_argument(
            "error[Add1D]: two blocks are not the same size");

    // set blockSize
    BfpBlock<F, I> res;
    res.blockSize = a.blockSize;

    // decide common exponent
    res.commonExp = a.commonExp + b.commonExp - BFP_BIAS;

    for (int i = 0; i < a.blockSize; i++) {
        // 1. multiply
        int64_t tempRes = (int64_t)a.mant[i] * (int64_t)b.mant[i];

        // 2. set sign
        res.sign.push_back(a.sign[i] ^ b.sign[i]);

        // 3. rounding to nearest even
        tempRes = RoundToNearestEven(tempRes, BFP_MANTISSA_LENGTH);

        // 4. check carry
        carry = std::max(
            carry, (uint16_t)(tempRes >> (int64_t)(BFP_MANTISSA_LENGTH +
                                                   BFP_MANTISSA_LENGTH + 1)));

        // 5. store result
        res.mant.push_back((I)(tempRes >> BFP_MANTISSA_LENGTH));
    }

    if (carry) {
        res.commonExp += carry;
        for (int i = 0; i < res.blockSize; i++) {
            res.mant[i] >>= carry;
        }
    }

    return res;
}

/* scalar-matrix arithmetic functions*/
template <class F, class I>
BfpBlock<F, I> AddScalar1D(float aScalar, BfpBlock<F, I> b) {
    uint16_t carry = 0;

    // set blockSize
    BfpBlock<F, I> res;
    res.blockSize = b.blockSize;

    // get a's sign, exponent, mantissa bits
    BfpNum<F, I> a(aScalar);

    // decide common exponent
    bool flag = a.exp >= b.commonExp ? 1 : 0;
    res.commonExp = flag ? a.exp : b.commonExp;
    int diff = std::abs(a.exp - b.commonExp);

    for (int i = 0; i < b.blockSize; i++) {
        // save mantissas in long long so that there is no data loss during
        // shifting
        int tempShiftNum =
            61 - BFP_MANTISSA_LENGTH;  // sign + carry + implicit1 + mantissa +
                                       // tempShiftNum = 64
        int64_t implicit1 = (int64_t)1 << 61;
        int64_t tempRes = (int64_t)0;
        int64_t tempA = (int64_t)a.mant << tempShiftNum;
        int64_t tempB = (int64_t)b.mant[i] << tempShiftNum;

        // align mantissas
        if (flag)  // if a's common exponent is bigger
            tempB >>= diff;
        else
            tempA >>= diff;

        // add mantissa
        // 1. conversion to 2's complement
        tempA = a.sign ? ~tempA + 1 : tempA;
        tempB = b.sign[i] ? ~tempB + 1 : tempB;

        // 2. add mantissas
        tempRes = tempA + tempB;

        // std::cout << i << std::endl;
        // std::cout << "   " << BitString<uint64_t>(tempA, 64) << "\t" <<
        // BitString<uint32_t>(a.mant[i], 24) << std::endl; std::cout << "+  "
        // << BitString<uint64_t>(tempB, 64) << "\t" <<
        // BitString<uint32_t>(b.mant[i], 24) << std::endl; std::cout <<
        // "----------------------------" << std::endl; std::cout << "   " <<
        // BitString<uint64_t>(tempRes, 64) << std::endl
        //           << std::endl;

        // 3. convert to signed magnitude if negative
        if (tempRes & 0x8000000000000000) {
            tempRes = ~tempRes + 1;
            res.sign.push_back((uint16_t)1);
        } else {
            res.sign.push_back((uint16_t)0);
        }

        // 4. rounding to nearest even
        tempRes = RoundToNearestEven(tempRes, tempShiftNum);

        // 5. check carry
        carry = std::max(carry, (uint16_t)(tempRes >> (int64_t)62));
        std::cout << carry << std::endl;

        // 6. store result
        res.mant.push_back((I)(tempRes >> tempShiftNum));
    }

    if (carry) {
        res.commonExp += carry;
        for (int i = 0; i < res.blockSize; i++) {
            res.mant[i] >>= carry;
        }
    }

    return res;
}

template <class F, class I>
BfpBlock<F, I> SubScalar1D(float aScalar, BfpBlock<F, I> b) {
    for (int i = 0; i < b.blockSize; i++) b.sign[i] ^= (uint16_t)1;

    BfpBlock<F, I> res = AddScalar1D<F, I>(aScalar, b);
    return res;
}

template <class F, class I>
BfpBlock<F, I> MultScalar1D(float aScalar, BfpBlock<F, I> b) {
    uint16_t carry = false;

    // get a's sign, exponent, mantissa bits
    BfpNum<F, I> a(aScalar);

    // set blockSize
    BfpBlock<F, I> res;
    res.blockSize = b.blockSize;

    // decide common exponent
    res.commonExp = a.exp + b.commonExp - BFP_BIAS;

    for (int i = 0; i < b.blockSize; i++) {
        // 1. multiply
        int64_t tempRes = (int64_t)a.mant * (int64_t)b.mant[i];

        // 2. set sign
        res.sign.push_back(a.sign ^ b.sign[i]);

        // 3. rounding to nearest even
        tempRes = RoundToNearestEven(tempRes, BFP_MANTISSA_LENGTH);

        // 4. check carry
        carry = std::max(
            carry, (uint16_t)(tempRes >> (int64_t)(BFP_MANTISSA_LENGTH +
                                                   BFP_MANTISSA_LENGTH + 1)));

        // 5. store result
        res.mant.push_back((I)(tempRes >> BFP_MANTISSA_LENGTH));
    }

    if (carry) {
        res.commonExp += carry;
        for (int i = 0; i < res.blockSize; i++) {
            res.mant[i] >>= carry;
        }
    }

    return res;
}

/* compare functions */
template <class F, class I>
BfpNum<F, I> Max(BfpBlock<F, I> b) {
    int64_t maxMant = 0x8000000000000000;

    for (int i = 0; i < b.blockSize; i++) {
        int64_t signedMant =
            b.sign[i] ? ~(int64_t)b.mant[i] + (int64_t)1 : (int64_t)b.mant[i];
        maxMant = signedMant > maxMant ? signedMant : maxMant;
    }

    if (maxMant & (int64_t)1 << 63)
        return BfpNum<F, I>((uint16_t)1, b.commonExp,
                            (I)(~maxMant + (int64_t)1));
    else
        return BfpNum<F, I>((uint16_t)0, b.commonExp, (I)maxMant);
}

template <class F, class I>
BfpNum<F, I> Min(BfpBlock<F, I> b) {
    int64_t minMant = 0x7FFFFFFFFFFFFFFF;

    for (int i = 0; i < b.blockSize; i++) {
        int64_t signedMant =
            b.sign[i] ? ~(int64_t)b.mant[i] + (int64_t)1 : (int64_t)b.mant[i];
        minMant = signedMant < minMant ? signedMant : minMant;
    }

    if (minMant & (int64_t)1 << 63)
        return BfpNum<F, I>((uint16_t)1, b.commonExp,
                            (I)(~minMant + (int64_t)1));
    else
        return BfpNum<F, I>((uint16_t)0, b.commonExp, (I)minMant);
}

template <class F, class I>
void Swap(BfpBlock<F, I> *a, BfpBlock<F, I> *b) {
    if (a->commonExp != b->commonExp) {
        // alter commonExp and mantissas before comparison
        if (a->commonExp > b->commonExp) {
            // align mantissas so that mantissas corresponds to the bigger
            // common exponent
            for (int i = 0; i < a->blockSize; i++) {
                b->mant[i] >>= (a->commonExp - b->commonExp);
            }

            // set smaller commonExp to the bigger commonExp
            b->commonExp = a->commonExp;
        } else {
            // align mantissas so that mantissas corresponds to the bigger
            // common exponent
            for (int i = 0; i < a->blockSize; i++) {
                a->mant[i] >>= (b->commonExp - a->commonExp);
            }

            // set smaller commonExp to the bigger commonExp
            a->commonExp = b->commonExp;
        }
    }

    // compare and swap (a: max block, b: min block)
    for (int i = 0; i < a->blockSize; i++) {
        int64_t signedAMant = a->sign[i] ? ~(int64_t)a->mant[i] + (int64_t)1
                                         : (int64_t)a->mant[i];
        int64_t signedBMant = b->sign[i] ? ~(int64_t)b->mant[i] + (int64_t)1
                                         : (int64_t)b->mant[i];

        if (signedBMant > signedAMant)  // compare
        {
            I mantTemp = b->mant[i];  // swap
            b->mant[i] = a->mant[i];
            a->mant[i] = mantTemp;

            uint16_t signTemp = b->sign[i];
            b->sign[i] = a->sign[i];
            a->sign[i] = signTemp;
        }
    }
}

/* print functions */
template <class F, class I>
void BfpBlock<F, I>::PrintBitwise() {
    std::cout << "--------------BFP block-------------" << std::endl;
    std::cout << "N: " << blockSize << std::endl;
    std::cout << "commonExp: "
              << BitString<uint16_t>(commonExp, BFP_EXPONENT_LENGTH)
              << std::endl;
    for (int i = 0; i < blockSize; i++) {
        BfpNum<F, I> b(sign[i], commonExp, mant[i]);
        std::cout << i << ": " << sign[i] << "\t"
                  << BitStringWithSpace<I>(mant[i], FIXED_POINT_LENGTH + 1);
        std::cout << "\t(" << b.ToFloatingPoint() << ")" << std::endl;
    }
    std::cout << std::endl;
}

}  // namespace pbrt
#include "bfpblock.h"

namespace pbrt {
/* block formatting */
BfpBlock::BfpBlock(std::vector<double> x) {
    // initialize some member variables
    blockSize = x.size();

    // check customized mantissa/exponent length is possible
    if (BFP_MANTISSA_LENGTH > 52)
        throw std::invalid_argument(
            "error: BFP_MANTISSA_LENGTH is too long for double!");
    else if (BFP_EXPONENT_LENGTH > 11)
        throw std::invalid_argument(
            "error: BFP_EXPONENT_LENGTH is too long for double!");

    // block formatting
    std::vector<uint16_t> exps;

    for (int i = 0; i < blockSize; i++) {
        // customize mantissa/exponent length
        BfpNum *temp = new BfpNum(x[i]);
        exps.push_back(temp->exp - DOUBLE_BIAS + BFP_BIAS);
        mant.push_back(
            temp->mant >>
            (DOUBLE_MANTISSA_LENGTH -
             BFP_MANTISSA_LENGTH));  // implicit 1 already added in BfpNum
        sign.push_back(temp->sign);
        delete temp;
    }

    // find and save common exponent
    uint16_t max_exp = 0;
    for (int i = 0; i < blockSize; i++) {
        if (exps[i] > max_exp) max_exp = exps[i];
    }
    commonExp = max_exp;

    // align mantissas
    for (int i = 0; i < blockSize; i++) mant[i] >>= (max_exp - exps[i]);
}

/* matrix element-wise arithmetic functions */
// this block: a, other block: b
BfpBlock BfpBlock::Add1D(BfpBlock b) {
    uint16_t carry = 0;
    BfpBlock res;

    // check if both blocks have same size
    if (blockSize != b.blockSize)
        throw std::invalid_argument(
            "error[Add1D]: two blocks are not the same size");

    // set blockSize
    res.blockSize = blockSize;

    // decide common exponent
    bool flag = commonExp >= b.commonExp ? 1 : 0;
    res.commonExp = flag ? commonExp : b.commonExp;
    int diff = std::abs(commonExp - b.commonExp);

    // shift mantissas in long long so that there is no data loss during
    // shifting
    int64_t tempImplicit1 = (int64_t)1 << 61;
    int64_t tempShiftNum =
        61 - BFP_MANTISSA_LENGTH;  // sign + carry + implicit1 + mantissa +
                                   // tempShiftNum = 64

    for (int i = 0; i < blockSize; i++) {
        int64_t tempRes = (int64_t)0;
        int64_t tempA = mant[i] << tempShiftNum;
        int64_t tempB = b.mant[i] << tempShiftNum;

        // align mantissas
        if (flag)  // if this block(a)'s common exponent is bigger
            tempB >>= diff;
        else
            tempA >>= diff;

        // add mantissa
        // 1. conversion to 2's complement
        tempA = sign[i] ? ~tempA + 1 : tempA;
        tempB = b.sign[i] ? ~tempB + 1 : tempB;

        // 2. add mantissas
        tempRes = tempA + tempB;

        // std::cout << i << std::endl;
        // std::cout << "   " << BitString<uint64_t>(tempA, 64) << "\t" <<
        // BitString<uint32_t>(mant[i], 24) << std::endl; std::cout << "+  "
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
        res.mant.push_back((uint64_t)(tempRes >> tempShiftNum));
    }

    if (carry) {
        res.commonExp += carry;
        for (int i = 0; i < blockSize; i++) {
            res.mant[i] >>= carry;
        }
    }

    return res;
}

BfpBlock BfpBlock::Sub1D(BfpBlock b) {
    for (int i = 0; i < blockSize; i++) b.sign[i] ^= (uint16_t)1;

    BfpBlock res = Add1D(b);
    return res;
}

BfpBlock BfpBlock::Mult1D(BfpBlock b) {
    uint16_t carry = 0;
    BfpBlock res;

    // check if both blocks have same size
    if (blockSize != b.blockSize) {
        throw std::invalid_argument(
            "error[Mult1D]: two blocks are not the same size");
    }

    // set blockSize
    res.blockSize = blockSize;

    // decide common exponent
    res.commonExp = commonExp + b.commonExp - BFP_BIAS;

    for (int i = 0; i < blockSize; i++) {
        // 1. multiply
        uint64_t tempRes = mant[i] * b.mant[i];

        // 2. set sign
        res.sign.push_back(sign[i] ^ b.sign[i]);

        // 3. rounding to nearest even
        tempRes = RoundToNearestEven(tempRes, BFP_MANTISSA_LENGTH);

        // 4. check carry
        carry = std::max(
            carry, (uint16_t)(tempRes >> (uint64_t)(BFP_MANTISSA_LENGTH +
                                                    BFP_MANTISSA_LENGTH + 1)));

        // 5. store result
        res.mant.push_back(tempRes >> BFP_MANTISSA_LENGTH);
    }

    if (carry) {
        res.commonExp += carry;
        for (int i = 0; i < blockSize; i++) {
            res.mant[i] >>= carry;
        }
    }

    return res;
}

BfpBlock BfpBlock::Div1D(BfpBlock b) {
    uint16_t carry = 0;
    BfpBlock res;

    // check if both blocks have same size
    if (blockSize != b.blockSize)
        throw std::invalid_argument(
            "error[Div1D]: two blocks are not the same size");

    // set blockSize
    res.blockSize = blockSize;

    // decide common exponent
    res.commonExp = commonExp - b.commonExp + BFP_BIAS;

    // save mantissas in long long so that there is no data loss during shifting
    uint64_t tempShiftNum =
        61 - BFP_MANTISSA_LENGTH;  // sign + carry + implicit1 + mantissa +
                                   // tempShiftNum = 64
    uint64_t tempImplicit1 = (uint64_t)1 << 61;
    for (int i = 0; i < blockSize; i++) {
        // 1. divide
        uint64_t tempA = mant[i] << tempShiftNum;
        uint64_t tempB = b.mant[i];
        uint64_t tempRes = tempA / tempB;

        // std::cout << "    " << BitStringWithSpace<int64_t>(tempA, 64)
        //           << std::endl;
        // std::cout << "/   " << BitStringWithSpace<int64_t>(tempB, 64)
        //           << std::endl;
        // std::cout << "------------------" << std::endl;
        // std::cout << "    " << BitStringWithSpace<int64_t>(tempRes, 64)
        //           << std::endl
        //           << std::endl;

        // 2. set sign
        res.sign.push_back(sign[i] ^ b.sign[i]);

        // 3. rounding to nearest even
        tempRes = RoundToNearestEven(tempRes, BFP_MANTISSA_LENGTH);

        // 4. check carry
        carry = std::max(
            carry, (uint16_t)(tempRes >> (uint64_t)(61 - BFP_MANTISSA_LENGTH)));

        // std::cout << carry << std::endl;

        // 5. store result
        res.mant.push_back(tempRes >> (uint64_t)(61 - BFP_MANTISSA_LENGTH -
                                                 BFP_MANTISSA_LENGTH));
    }

    if (carry) {
        res.commonExp += carry;
        for (int i = 0; i < res.blockSize; i++) {
            res.mant[i] >>= (uint64_t)carry;
        }
    }

    return res;
}

/* scalar-matrix arithmetic functions*/
BfpBlock BfpBlock::AddScalar1D(double scalar) {
    uint16_t carry = 0;
    BfpBlock res;

    // set blockSize
    res.blockSize = blockSize;

    // get a's sign, exponent, mantissa bits
    BfpNum s(scalar);

    // adjust exponent, mantissa into bfp precision
    s.exp = s.exp - DOUBLE_BIAS + BFP_BIAS;
    s.mant >>= (DOUBLE_MANTISSA_LENGTH - BFP_MANTISSA_LENGTH);

    // decide common exponent
    bool flag = s.exp >= commonExp ? 1 : 0;
    res.commonExp = flag ? s.exp : commonExp;
    int diff = std::abs(s.exp - commonExp);

    // shift mantissas in long long so that there is no data loss during
    // shifting
    uint64_t tempImplicit1 = (uint64_t)1 << 61;
    uint64_t tempShiftNum =
        61 - BFP_MANTISSA_LENGTH;  // sign + carry + implicit1 + mantissa +
                                   // tempShiftNum = 64

    for (int i = 0; i < blockSize; i++) {
        uint64_t tempRes = (uint64_t)0;
        uint64_t tempS = s.mant << tempShiftNum;
        uint64_t tempB = mant[i] << tempShiftNum;

        // align mantissas
        if (flag)  // if a's common exponent is bigger
            tempB >>= diff;
        else
            tempS >>= diff;

        // add mantissa
        // 1. conversion to 2's complement
        tempS = s.sign ? ~tempS + 1 : tempS;
        tempB = sign[i] ? ~tempB + 1 : tempB;

        // 2. add mantissas
        tempRes = tempS + tempB;

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
        carry = std::max(carry, (uint16_t)(tempRes >> (uint64_t)62));
        // std::cout << carry << std::endl;

        // 6. store result
        res.mant.push_back(tempRes >> tempShiftNum);
    }

    if (carry) {
        res.commonExp += carry;
        for (int i = 0; i < res.blockSize; i++) {
            res.mant[i] >>= carry;
        }
    }

    return res;
}

BfpBlock BfpBlock::SubScalar1D(double scalar, bool isScalarFirst) {
    BfpBlock res;
    if (isScalarFirst)  // scalar - bfp
    {
        for (int i = 0; i < blockSize; i++) sign[i] ^= (uint16_t)1;

        res = AddScalar1D(scalar);
    } else  // bfp - scalar
    {
        res = AddScalar1D(-scalar);
    }
    return res;
}

BfpBlock BfpBlock::MultScalar1D(double scalar) {
    uint16_t carry = false;
    BfpBlock res;

    // get a's sign, exponent, mantissa bits
    BfpNum s(scalar);

    // adjust exponent, mantissa into bfp precision
    s.exp = s.exp - DOUBLE_BIAS + BFP_BIAS;
    s.mant >>= (DOUBLE_MANTISSA_LENGTH - BFP_MANTISSA_LENGTH);

    // set blockSize
    res.blockSize = blockSize;

    // decide common exponent
    res.commonExp = s.exp + commonExp - BFP_BIAS;

    for (int i = 0; i < blockSize; i++) {
        // 1. multiply
        uint64_t tempRes = s.mant * mant[i];

        // 2. set sign
        res.sign.push_back(s.sign ^ sign[i]);

        // 3. rounding to nearest even
        tempRes = RoundToNearestEven(tempRes, BFP_MANTISSA_LENGTH);

        // 4. check carry
        carry = std::max(
            carry, (uint16_t)(tempRes >> (uint64_t)(BFP_MANTISSA_LENGTH +
                                                    BFP_MANTISSA_LENGTH + 1)));

        // 5. store result
        res.mant.push_back(tempRes >> BFP_MANTISSA_LENGTH);
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
BfpNum BfpBlock::Max() {
    int64_t maxMant = 0x8000000000000000;

    for (int i = 0; i < blockSize; i++) {
        int64_t signedMant = sign[i] ? ~mant[i] + (int64_t)1 : mant[i];
        maxMant = signedMant > maxMant ? signedMant : maxMant;
    }

    if (maxMant & (int64_t)1 << 63)
        return BfpNum((uint16_t)1, commonExp,
                      (uint64_t)(~maxMant + (int64_t)1));
    else
        return BfpNum((uint16_t)0, commonExp, maxMant);
}

BfpNum BfpBlock::Min() {
    int64_t minMant = 0x7FFFFFFFFFFFFFFF;

    for (int i = 0; i < blockSize; i++) {
        int64_t signedMant =
            sign[i] ? ~(int64_t)mant[i] + (int64_t)1 : (int64_t)mant[i];
        minMant = signedMant < minMant ? signedMant : minMant;
    }

    if (minMant & (int64_t)1 << 63)
        return BfpNum((uint16_t)1, commonExp,
                      (uint64_t)(~minMant + (int64_t)1));
    else
        return BfpNum((uint16_t)0, commonExp, (uint64_t)minMant);
}

void BfpBlock::Swap(BfpBlock *b) {
    // check if both blocks have same size
    if (blockSize != b->blockSize)
        throw std::invalid_argument(
            "error[Swap]: two blocks are not the same size");

    // alter commonExp and mantissas before comparison
    if (commonExp > b->commonExp) {
        // align mantissas so that mantissas corresponds to the bigger
        // common exponent
        for (int i = 0; i < blockSize; i++) {
            b->mant[i] >>= (commonExp - b->commonExp);
        }

        // set smaller commonExp to the bigger commonExp
        b->commonExp = commonExp;
    } else if (b->commonExp > commonExp) {
        // align mantissas so that mantissas corresponds to the bigger
        // common exponent
        for (int i = 0; i < blockSize; i++) {
            mant[i] >>= (b->commonExp - commonExp);
        }

        // set smaller commonExp to the bigger commonExp
        commonExp = b->commonExp;
    }

    // compare and swap (a: max block, b: min block)
    for (int i = 0; i < blockSize; i++) {
        int64_t signedAMant =
            sign[i] ? ~(int64_t)mant[i] + (int64_t)1 : (int64_t)mant[i];
        int64_t signedBMant = b->sign[i] ? ~(int64_t)b->mant[i] + (int64_t)1
                                         : (int64_t)b->mant[i];

        if (signedBMant > signedAMant)  // compare
        {
            uint64_t mantTemp = b->mant[i];  // swap
            b->mant[i] = mant[i];
            mant[i] = mantTemp;

            uint16_t signTemp = b->sign[i];
            b->sign[i] = sign[i];
            sign[i] = signTemp;
        }
    }
}

/* print functions */
void BfpBlock::PrintBitwise() {
    std::cout << "--------------BFP block-------------" << std::endl;
    std::cout << "N: " << blockSize << std::endl;
    std::cout << "commonExp: " << BitString(commonExp, BFP_EXPONENT_LENGTH)
              << std::endl;
    for (int i = 0; i < blockSize; i++) {
        BfpNum b(sign[i], commonExp, mant[i]);
        std::cout << i << ": " << sign[i] << "\t"
                  << BitStringWithSpace(mant[i], FIXED_POINT_LENGTH);
        std::cout << "\t(" << b.ToFloatingPoint() << ")" << std::endl;
    }
    std::cout << std::endl;
}

std::vector<double> BfpBlock::ToFloatingPoint() {
    std::vector<double> res;
    for (int i = 0; i < blockSize; i++) {
        BfpNum b(sign[i], commonExp, mant[i]);
        res.push_back(b.ToFloatingPoint());
    }
    return res;
}

}  // namespace pbrt
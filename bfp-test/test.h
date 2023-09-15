
#include <stdint.h>

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "bfpblock.h"
#include "bfpnum.h"
#include "bfputility.h"
#include "bfpimpl.h"

namespace pbrt
{

    void printVectorBitwise(std::vector<float> a)
    {
        for (int i = 0; i < a.size(); i++)
        {
            BfpNum<float, uint32_t> aBitwise(a[i]);
            aBitwise.PrintBitwise();
        }
    }

    void testBfpBlock(std::vector<float> a)
    {
        BfpBlock<float, uint32_t> aBlock(a);
        aBlock.PrintBitwise();
    }

    void testBfpBlockAdd1D(std::vector<float> a, std::vector<float> b)
    {
        BfpBlock<float, uint32_t> aBlock(a);
        BfpBlock<float, uint32_t> bBlock(b);
        BfpBlock<float, uint32_t> res = Add1D<float, uint32_t>(aBlock, bBlock);
        aBlock.PrintBitwise();
        bBlock.PrintBitwise();
        res.PrintBitwise();

        std::cout << "-------------compare-------------" << std::endl;
        for (int i = 0; i < a.size(); i++)
        {
            std::cout << i << ": " << a[i] + b[i] << std::endl;
        }
    }

    void testBfpBlockSub1D(std::vector<float> a, std::vector<float> b)
    {
        BfpBlock<float, uint32_t> aBlock(a);
        BfpBlock<float, uint32_t> bBlock(b);
        BfpBlock<float, uint32_t> res = Sub1D<float, uint32_t>(aBlock, bBlock);
        aBlock.PrintBitwise();
        bBlock.PrintBitwise();
        res.PrintBitwise();

        std::cout << "-------------compare-------------" << std::endl;
        for (int i = 0; i < a.size(); i++)
        {
            std::cout << i << ": " << a[i] - b[i] << std::endl;
        }
    }

    void testBfpBlockMult1D(std::vector<float> a, std::vector<float> b)
    {
        BfpBlock<float, uint32_t> aBlock(a);
        BfpBlock<float, uint32_t> bBlock(b);
        BfpBlock<float, uint32_t> res = Mult1D<float, uint32_t>(aBlock, bBlock);
        aBlock.PrintBitwise();
        bBlock.PrintBitwise();
        res.PrintBitwise();

        std::cout << "-------------compare-------------" << std::endl;
        for (int i = 0; i < a.size(); i++)
        {
            std::cout << i << ": " << a[i] * b[i] << std::endl;
        }
    }

    void testBfpBlockDiv1D(std::vector<float> a, std::vector<float> b)
    {
        BfpBlock<float, uint32_t> aBlock(a);
        BfpBlock<float, uint32_t> bBlock(b);
        BfpBlock<float, uint32_t> res = Div1D<float, uint32_t>(aBlock, bBlock);
        aBlock.PrintBitwise();
        bBlock.PrintBitwise();
        res.PrintBitwise();

        std::cout << "-------------compare-------------" << std::endl;
        for (int i = 0; i < a.size(); i++)
        {
            std::cout << i << ": " << a[i] / b[i] << std::endl;
        }
    }

    void testAddScalar1D(float aScalar, std::vector<float> b)
    {
        BfpBlock<float, uint32_t> bBlock(b);
        BfpBlock<float, uint32_t> res = AddScalar1D<float, uint32_t>(aScalar, bBlock);
        BfpNum<float, uint32_t> a(aScalar);
        a.PrintBitwise();
        bBlock.PrintBitwise();
        res.PrintBitwise();

        std::cout << "-------------compare-------------" << std::endl;
        for (int i = 0; i < b.size(); i++)
        {
            std::cout << i << ": " << aScalar + b[i] << std::endl;
        }
    }

    void testSubScalar1D(float aScalar, std::vector<float> b)
    {
        BfpBlock<float, uint32_t> bBlock(b);
        BfpBlock<float, uint32_t> res = SubScalar1D<float, uint32_t>(aScalar, bBlock);
        BfpNum<float, uint32_t> a(aScalar);
        a.PrintBitwise();
        bBlock.PrintBitwise();
        res.PrintBitwise();

        std::cout << "-------------compare-------------" << std::endl;
        for (int i = 0; i < b.size(); i++)
        {
            std::cout << i << ": " << aScalar - b[i] << std::endl;
        }
    }

    void testMultScalar1D(float aScalar, std::vector<float> b)
    {
        BfpBlock<float, uint32_t> bBlock(b);
        BfpBlock<float, uint32_t> res = MultScalar1D<float, uint32_t>(aScalar, bBlock);
        BfpNum<float, uint32_t> a(aScalar);
        a.PrintBitwise();
        std::cout << "\nx\n"
                  << std::endl;
        bBlock.PrintBitwise();
        std::cout << "=================================================================" << std::endl;
        res.PrintBitwise();

        std::cout << "-------------compare with actual fp value-------------"
                  << std::endl;
        for (int i = 0; i < b.size(); i++)
        {
            std::cout << i << ": " << aScalar * b[i] << std::endl;
        }
    }

    void testMax(std::vector<float> a)
    {
        BfpBlock<float, uint32_t> aBlock(a);
        aBlock.PrintBitwise();
        std::cout << "\n\n======================<MAX VALUE>================"
                  << std::endl;
        BfpNum<float, uint32_t> res = pbrt::Max(aBlock);
        res.PrintBitwise();
        res.PrintValue();
    }

    void testMin(std::vector<float> a)
    {
        BfpBlock<float, uint32_t> aBlock(a);
        aBlock.PrintBitwise();
        std::cout << "\n\n=====================<MIN VALUE>==================="
                  << std::endl;
        BfpNum<float, uint32_t> res = pbrt::Min(aBlock);
        res.PrintBitwise();
        res.PrintValue();
    }

    void testSwap(std::vector<float> a, std::vector<float> b)
    {
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
} // namespace pbrt

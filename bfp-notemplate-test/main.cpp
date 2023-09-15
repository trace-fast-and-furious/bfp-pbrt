#include "./bfputility.h"
#include "./bfpnum.h"
#include "./bfpblock.h"

using namespace pbrt;

void printVectorBitwise(std::vector<double> a)
{
    for (int i = 0; i < a.size(); i++)
    {
        BfpNum aBitwise(a[i]);
        aBitwise.PrintBitwise();
    }
}

// void testBfpBlock(std::vector<double> a)
// {
//     BfpBlock aBlock(a);
//     aBlock.PrintBitwise();
// }

// void testBfpBlockAdd1D(std::vector<double> a, std::vector<double> b)
// {
//     BfpBlock aBlock(a);
//     BfpBlock bBlock(b);
//     BfpBlock res = aBlock.Add1D(bBlock);
//     aBlock.PrintBitwise();
//     bBlock.PrintBitwise();
//     res.PrintBitwise();

//     std::cout << "-------------compare-------------" << std::endl;
//     for (int i = 0; i < a.size(); i++)
//     {
//         std::cout << i << ": " << a[i] + b[i] << std::endl;
//     }
// }

// void testBfpBlockSub1D(std::vector<double> a, std::vector<double> b)
// {
//     BfpBlock aBlock(a);
//     BfpBlock bBlock(b);
//     BfpBlock res = aBlock.Sub1D(bBlock);
//     aBlock.PrintBitwise();
//     bBlock.PrintBitwise();
//     res.PrintBitwise();

//     std::cout << "-------------compare-------------" << std::endl;
//     for (int i = 0; i < a.size(); i++)
//     {
//         std::cout << i << ": " << a[i] - b[i] << std::endl;
//     }
// }

// void testBfpBlockMult1D(std::vector<double> a, std::vector<double> b)
// {
//     BfpBlock aBlock(a);
//     BfpBlock bBlock(b);
//     BfpBlock res = aBlock.Mult1D(bBlock);
//     aBlock.PrintBitwise();
//     bBlock.PrintBitwise();
//     res.PrintBitwise();

//     std::cout << "-------------compare-------------" << std::endl;
//     for (int i = 0; i < a.size(); i++)
//     {
//         std::cout << i << ": " << a[i] * b[i] << std::endl;
//     }
// }

// void testBfpBlockDiv1D(std::vector<double> a, std::vector<double> b)
// {
//     BfpBlock aBlock(a);
//     BfpBlock bBlock(b);
//     BfpBlock res = aBlock.Div1D(bBlock);
//     aBlock.PrintBitwise();
//     bBlock.PrintBitwise();
//     res.PrintBitwise();

//     std::cout << "-------------compare-------------" << std::endl;
//     for (int i = 0; i < a.size(); i++)
//     {
//         std::cout << i << ": " << a[i] / b[i] << std::endl;
//     }
// }

// void testAddScalar1D(double aScalar, std::vector<double> b)
// {
//     BfpBlock bBlock(b);
//     BfpBlock res = bBlock.AddScalar1D(aScalar);
//     BfpNum a(aScalar);
//     a.PrintBitwise();
//     bBlock.PrintBitwise();
//     res.PrintBitwise();

//     std::cout << "-------------compare-------------" << std::endl;
//     for (int i = 0; i < b.size(); i++)
//     {
//         std::cout << i << ": " << aScalar + b[i] << std::endl;
//     }
// }

// void testSubScalar1D(double aScalar, std::vector<double> b)
// {
//     BfpBlock bBlock(b);
//     BfpBlock res = bBlock.SubScalar1D(aScalar, true);
//     BfpNum a(aScalar);
//     a.PrintBitwise();
//     bBlock.PrintBitwise();
//     res.PrintBitwise();

//     std::cout << "-------------compare-------------" << std::endl;
//     for (int i = 0; i < b.size(); i++)
//     {
//         std::cout << i << ": " << aScalar - b[i] << std::endl;
//     }
// }

// void testMultScalar1D(double aScalar, std::vector<double> b)
// {
//     BfpBlock bBlock(b);
//     BfpBlock res = bBlock.MultScalar1D(aScalar);
//     BfpNum a(aScalar);
//     a.PrintBitwise();
//     std::cout << "\nx\n"
//               << std::endl;
//     bBlock.PrintBitwise();
//     std::cout << "=================================================================" << std::endl;
//     res.PrintBitwise();

//     std::cout << "-------------compare with actual fp value-------------" << std::endl;
//     for (int i = 0; i < b.size(); i++)
//     {
//         std::cout << i << ": " << aScalar * b[i] << std::endl;
//     }
// }

// void testMax(std::vector<double> a)
// {
//     BfpBlock aBlock(a);
//     aBlock.PrintBitwise();
//     std::cout << "\n\n======================<MAX VALUE>================"
//               << std::endl;
//     BfpNum res = aBlock.Max();
//     res.PrintBitwise();
//     res.PrintValue();
// }

// void testMin(std::vector<double> a)
// {
//     BfpBlock aBlock(a);
//     aBlock.PrintBitwise();
//     std::cout << "\n\n=====================<MIN VALUE>==================="
//               << std::endl;
//     BfpNum res = aBlock.Min();
//     res.PrintBitwise();
//     res.PrintValue();
// }

// void testSwap(std::vector<double> a, std::vector<double> b)
// {
//     BfpBlock aBlock(a);
//     BfpBlock bBlock(b);
//     std::cout << "\n================<BEFORE SWAP>==============" << std::endl;
//     aBlock.PrintBitwise();
//     bBlock.PrintBitwise();
//     aBlock.Swap(&bBlock);
//     std::cout << "\n\n================<AFTER SWAP>==============" << std::endl;
//     aBlock.PrintBitwise();
//     bBlock.PrintBitwise();
// }

int main(void)
{
    std::vector<double> a{-1.0, 1.5, 1.75, 1.00805664062, -1.00805699825};
    std::vector<double> b{-0.5, -1.5, 1.75, 0.00805664062, 1.00805699825};
    std::vector<double> c{0.01171875, 0.013671875, 0.0146484375, 0.01513671875,
                          0.015380859375}; // same exponent: 01111000

    // printVectorBitwise(a);
    BfpBlock aBlock(a);
    return 0;
}

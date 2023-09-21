#include "./bfputility.h"
#include "./bfpnum.h"
#include "./bfpblock.h"

#include <iomanip>

using namespace pbrt;

void compareResults(std::vector<double> b, std::vector<double> d)
{
    std::cout << "================compare================" << std::endl;
    std::cout << std::left << std::setw(3) << "idx"
              << "|" << std::setw(15) << "BFP value"
              << "|" << std::setw(15) << "original value"
              << "|" << std::setw(15) << "compare" << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
    for (int i = 0; i < b.size(); i++)
    {
        std::cout << std::left << std::setw(3) << i << "|" << std::setw(15) << b[i] << "|" << std::setw(15) << d[i];
        if (std::abs(b[i] - d[i]) < std::numeric_limits<double>::epsilon())
            std::cout << "|" << std::setw(15) << "same" << std::endl;
        else if (std::abs(b[i] - d[i]) < 0.000001)
            std::cout << "|" << std::setw(15) << "similar" << std::endl;
        else
            std::cout << "|" << std::setw(15) << "DIFFERENT" << std::endl;
    }
}

void compareResults(double a, double b)
{
    std::cout << "================compare================" << std::endl;
    std::cout << std::left << std::setw(15) << "BFP value"
              << "|" << std::setw(15) << "original value"
              << "|" << std::setw(15) << "compare" << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << std::left << std::setw(15) << a << "|" << std::setw(15) << b;
    if (std::abs(a - b) < std::numeric_limits<double>::epsilon())
        std::cout << "|" << std::setw(15) << "same" << std::endl;
    else if (std::abs(a - b) < 0.000001)
        std::cout << "|" << std::setw(15) << "similar" << std::endl;
    else
        std::cout << "|" << std::setw(15) << "DIFFERENT" << std::endl;
}

void printVectorBitwise(std::vector<double> a)
{
    for (int i = 0; i < a.size(); i++)
    {
        BfpNum aBitwise(a[i]);
        aBitwise.PrintBitwise();
    }
}

void testBfpBlock(std::vector<double> a)
{
    BfpBlock aBlock(a);
    aBlock.PrintBitwise();
}

void testBfpBlockAdd1D(std::vector<double> a, std::vector<double> b)
{
    BfpBlock aBlock(a);
    BfpBlock bBlock(b);
    BfpBlock res = aBlock.Add1D(bBlock);
    aBlock.PrintBitwise();
    bBlock.PrintBitwise();
    res.PrintBitwise();
    std::vector<double> original;

    for (int i = 0; i < a.size(); i++)
    {
        original.push_back(a[i] + b[i]);
    }
    compareResults(res.ToFloatingPoint(), original);
}

void testBfpBlockSub1D(std::vector<double> a, std::vector<double> b)
{
    BfpBlock aBlock(a);
    BfpBlock bBlock(b);
    BfpBlock res = aBlock.Sub1D(bBlock);
    aBlock.PrintBitwise();
    bBlock.PrintBitwise();
    res.PrintBitwise();
    std::vector<double> original;

    for (int i = 0; i < a.size(); i++)
    {
        original.push_back(a[i] - b[i]);
    }
    compareResults(res.ToFloatingPoint(), original);
}

void testBfpBlockMult1D(std::vector<double> a, std::vector<double> b)
{
    BfpBlock aBlock(a);
    BfpBlock bBlock(b);
    BfpBlock res = aBlock.Mult1D(bBlock);
    aBlock.PrintBitwise();
    bBlock.PrintBitwise();
    res.PrintBitwise();
    std::vector<double> original;

    for (int i = 0; i < a.size(); i++)
    {
        original.push_back(a[i] * b[i]);
    }
    compareResults(res.ToFloatingPoint(), original);
}

void testBfpBlockDiv1D(std::vector<double> a, std::vector<double> b)
{
    BfpBlock aBlock(a);
    BfpBlock bBlock(b);
    BfpBlock res = aBlock.Div1D(bBlock);
    aBlock.PrintBitwise();
    bBlock.PrintBitwise();
    res.PrintBitwise();
    std::vector<double> original;

    for (int i = 0; i < a.size(); i++)
    {
        original.push_back(a[i] / b[i]);
    }
    compareResults(res.ToFloatingPoint(), original);
}

void testAddScalar1D(double aScalar, std::vector<double> b)
{
    BfpBlock bBlock(b);
    BfpBlock res = bBlock.AddScalar1D(aScalar);
    BfpNum a(aScalar);
    a.PrintBitwise();
    bBlock.PrintBitwise();
    res.PrintBitwise();
    std::vector<double> original;

    for (int i = 0; i < b.size(); i++)
    {
        original.push_back(aScalar + b[i]);
    }
    compareResults(res.ToFloatingPoint(), original);
}

void testSubScalar1D(double aScalar, std::vector<double> b, bool isScalarFirst)
{
    BfpBlock bBlock(b);
    BfpBlock res = bBlock.SubScalar1D(aScalar, isScalarFirst);
    BfpNum a(aScalar);
    a.PrintBitwise();
    bBlock.PrintBitwise();
    res.PrintBitwise();
    std::vector<double> original;

    for (int i = 0; i < b.size(); i++)
    {
        if (isScalarFirst)
            original.push_back(aScalar - b[i]);
        else
            original.push_back(b[i] - aScalar);
    }
    compareResults(res.ToFloatingPoint(), original);
}

void testMultScalar1D(double aScalar, std::vector<double> b)
{
    BfpBlock bBlock(b);
    BfpBlock res = bBlock.MultScalar1D(aScalar);
    BfpNum a(aScalar);
    a.PrintBitwise();
    bBlock.PrintBitwise();
    res.PrintBitwise();

    std::vector<double> original;
    for (int i = 0; i < b.size(); i++)
    {
        original.push_back(aScalar * b[i]);
    }
    compareResults(res.ToFloatingPoint(), original);
}

void testMax(std::vector<double> a)
{
    BfpBlock aBlock(a);
    aBlock.PrintBitwise();
    BfpNum res = aBlock.Max();
    res.PrintBitwise();
    std::cout << std::endl;
    compareResults(res.ToFloatingPoint(), *max_element(a.begin(), a.end()));
}

void testMin(std::vector<double> a)
{
    BfpBlock aBlock(a);
    aBlock.PrintBitwise();
    BfpNum res = aBlock.Min();
    res.PrintBitwise();
    std::cout << std::endl;
    compareResults(res.ToFloatingPoint(), *min_element(a.begin(), a.end()));
}

void testSwap(std::vector<double> a, std::vector<double> b)
{
    BfpBlock aBlock(a);
    BfpBlock bBlock(b);
    std::cout << "\n================<BEFORE SWAP>==============" << std::endl;
    aBlock.PrintBitwise();
    bBlock.PrintBitwise();
    aBlock.Swap(&bBlock);
    std::cout << "\n\n================<AFTER SWAP>==============" << std::endl;
    aBlock.PrintBitwise();
    bBlock.PrintBitwise();
}

int main(void)
{
    std::vector<double> a{-1.0, 1.5, 1.75, 1.00805664062, -1.00805699825};
    std::vector<double> b{-0.5, -1.5, 1.75, 0.00805664062, 1.00805699825};
    std::vector<double> c{0.01171875, 0.013671875, 0.0146484375, 0.01513671875, 0.015380859375}; // same exponent: 01111000

    // printVectorBitwise(b);

    testSwap(b, c);
    return 0;
}

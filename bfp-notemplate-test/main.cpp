#include "./bfputility.h"
#include "./bfpnum.h"
#include "./bfpblock.h"

using namespace pbrt;

void compareResults(std::vector<std::vector<double>> b, std::vector<std::vector<double>> d)
{
    std::cout << "================compare================" << std::endl;
    for (int i = 0; i < b.size(); i++)
    {
        std::cout << i << std::endl;
        for (int j = 0; j < b[0].size(); j++)
        {
            std::cout
                << "   | " << j << ": " << std::setw(15) << b[i][j] << " vs " << std::setw(15) << d[i][j];
            if (std::abs(b[i][j] - d[i][j]) < std::numeric_limits<double>::epsilon())
                std::cout << " | "
                          << "same" << std::endl;
            else if (std::abs(b[i][j] - d[i][j]) < 0.000001)
                std::cout << " | "
                          << "similar" << std::endl;
            else
                std::cout << " | "
                          << "DIFFERENT" << std::endl;
        }
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

void printVectorBitwise(std::vector<std::vector<double>> a)
{
    for (int i = 0; i < a.size(); i++)
    {
        std::cout << i << std::endl;
        for (int j = 0; j < a[0].size(); j++)
        {
            std::cout
                << "   | " << j << ":   ";
            BfpNum aBitwise(a[i][j]);
            aBitwise.PrintBitwise();
        }
    }
}

void testBfpBlock(std::vector<std::vector<double>> a)
{
    BfpBlock aBlock(a);
    aBlock.PrintBitwise();
}

void testBfpBlockAdd(std::vector<std::vector<double>> a, std::vector<std::vector<double>> b, bool printBitwise)
{
    BfpBlock aBlock(a);
    BfpBlock bBlock(b);
    BfpBlock res = aBlock.Add(bBlock);
    if (printBitwise)
    {
        aBlock.PrintBitwise();
        bBlock.PrintBitwise();
        res.PrintBitwise();
    }

    std::vector<std::vector<double>> original;

    for (int i = 0; i < a.size(); i++)
    {
        std::vector<double> row;
        for (int j = 0; j < a[0].size(); j++)
        {
            row.push_back(a[i][j] + b[i][j]);
        }
        original.push_back(row);
    }
    compareResults(res.ToFloatingPoint(), original);
}

void testBfpBlockSub(std::vector<std::vector<double>> a, std::vector<std::vector<double>> b, bool printBitwise)
{
    BfpBlock aBlock(a);
    BfpBlock bBlock(b);
    BfpBlock res = aBlock.Sub(bBlock);
    if (printBitwise)
    {
        aBlock.PrintBitwise();
        bBlock.PrintBitwise();
        res.PrintBitwise();
    }

    std::vector<std::vector<double>> original;

    for (int i = 0; i < a.size(); i++)
    {
        std::vector<double> row;
        for (int j = 0; j < a[0].size(); j++)
        {
            row.push_back(a[i][j] - b[i][j]);
        }
        original.push_back(row);
    }
    compareResults(res.ToFloatingPoint(), original);
}

void testBfpBlockMult(std::vector<std::vector<double>> a, std::vector<std::vector<double>> b, bool printBitwise)
{
    BfpBlock aBlock(a);
    BfpBlock bBlock(b);
    BfpBlock res = aBlock.Mult(bBlock);
    if (printBitwise)
    {
        aBlock.PrintBitwise();
        bBlock.PrintBitwise();
        res.PrintBitwise();
    }

    std::vector<std::vector<double>> original;

    for (int i = 0; i < a.size(); i++)
    {
        std::vector<double> row;
        for (int j = 0; j < a[0].size(); j++)
        {
            row.push_back(a[i][j] * b[i][j]);
        }
        original.push_back(row);
    }
    compareResults(res.ToFloatingPoint(), original);
}

// void testBfpBlockDiv1D(std::vector<double> a, std::vector<double> b)
// {
//     BfpBlock aBlock(a);
//     BfpBlock bBlock(b);
//     BfpBlock res = aBlock.Div1D(bBlock);
//     aBlock.PrintBitwise();
//     bBlock.PrintBitwise();
//     res.PrintBitwise();
//     std::vector<double> original;

//     for (int i = 0; i < a.size(); i++)
//     {
//         original.push_back(a[i] / b[i]);
//     }
//     compareResults(res.ToFloatingPoint(), original);
// }

void testAddScalar(double aScalar, std::vector<std::vector<double>> b, bool printBitwise)
{
    BfpNum a(aScalar);
    BfpBlock bBlock(b);
    BfpBlock res = bBlock.AddScalar(aScalar);
    if (printBitwise)
    {
        a.PrintBitwise();
        bBlock.PrintBitwise();
        res.PrintBitwise();
    }

    std::vector<std::vector<double>> original;

    for (int i = 0; i < b.size(); i++)
    {
        std::vector<double> row;
        for (int j = 0; j < b[0].size(); j++)
        {
            row.push_back(aScalar + b[i][j]);
        }
        original.push_back(row);
    }
    compareResults(res.ToFloatingPoint(), original);
}

void testSubScalar(double aScalar, std::vector<std::vector<double>> b, bool isScalarFirst, bool printBitwise)
{
    BfpNum a(aScalar);
    BfpBlock bBlock(b);
    BfpBlock res = bBlock.SubScalar(aScalar, isScalarFirst);
    if (printBitwise)
    {
        a.PrintBitwise();
        bBlock.PrintBitwise();
        res.PrintBitwise();
    }

    std::vector<std::vector<double>> original;

    for (int i = 0; i < b.size(); i++)
    {
        std::vector<double> row;
        for (int j = 0; j < b[0].size(); j++)
        {
            if (isScalarFirst)
                row.push_back(aScalar - b[i][j]);
            else
                row.push_back(b[i][j] - aScalar);
        }
        original.push_back(row);
    }
    compareResults(res.ToFloatingPoint(), original);
}

void testMultScalar(double aScalar, std::vector<std::vector<double>> b, bool printBitwise)
{
    BfpNum a(aScalar);
    BfpBlock bBlock(b);
    BfpBlock res = bBlock.MultScalar(aScalar);
    if (printBitwise)
    {
        a.PrintBitwise();
        bBlock.PrintBitwise();
        res.PrintBitwise();
    }

    std::vector<std::vector<double>> original;

    for (int i = 0; i < b.size(); i++)
    {
        std::vector<double> row;
        for (int j = 0; j < b[0].size(); j++)
        {
            row.push_back(aScalar * b[i][j]);
        }
        original.push_back(row);
    }
    compareResults(res.ToFloatingPoint(), original);
}

void testMax(std::vector<std::vector<double>> a, bool printBitwise)
{
    BfpBlock aBlock(a);
    BfpNum res = aBlock.Max();
    if (printBitwise)
    {
        aBlock.PrintBitwise();
        res.PrintBitwise();
        std::cout << std::endl;
    }
    double maxValue = 0;
    for (int i = 0; i < a.size(); i++)
    {
        maxValue = std::max(maxValue, *max_element(a[i].begin(), a[i].end()));
    }
    compareResults(res.ToFloatingPoint(), maxValue);
}

void testMin(std::vector<std::vector<double>> a, bool printBitwise)
{
    BfpBlock aBlock(a);
    BfpNum res = aBlock.Min();
    if (printBitwise)
    {
        aBlock.PrintBitwise();
        res.PrintBitwise();
        std::cout << std::endl;
    }
    double minValue = std::numeric_limits<double>::max();
    for (int i = 0; i < a.size(); i++)
    {
        minValue = std::min(minValue, *min_element(a[i].begin(), a[i].end()));
    }
    compareResults(res.ToFloatingPoint(), minValue);
}

void testSwap(std::vector<std::vector<double>> a, std::vector<std::vector<double>> b)
{
    BfpBlock aBlock(a);
    BfpBlock bBlock(b);
    std::cout << "\n================<BEFORE SWAP>==============" << std::endl;
    aBlock.PrintValue();
    bBlock.PrintValue();
    aBlock.Swap(&bBlock);
    std::cout << "\n\n================<AFTER SWAP>==============" << std::endl;
    aBlock.PrintValue();
    bBlock.PrintValue();
}

int main(void)
{
    std::vector<std::vector<double>> a{{-1.0, 1.5}, {1.75, 1.00805664062}, {-1.75, -1.00805699825}};
    std::vector<std::vector<double>> b{{-0.5, -1.5}, {1.75, 0.00805664062}, {1.00805699825, 1.00805699825}};
    std::vector<std::vector<double>> c{{0.01171875, 0.013671875}, {0.0146484375, 0.01513671875}, {0.015380859375, 0.015380859375}}; // same exponent: 01111000

    testMultScalar(0.01171875, c, false);
    return 0;
}

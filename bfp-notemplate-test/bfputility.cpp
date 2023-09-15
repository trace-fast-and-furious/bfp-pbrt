#include "bfputility.h"

namespace pbrt
{
    /* print functions */
    void printBit(uint64_t num, int len)
    {
        char out[100] = "";
        for (int i = 0; i < len; i++, num >>= 1)
        {
            if (num & 1)
                std::strcat(out, "1");
            else
                std::strcat(out, "0");
        }
        for (int i = len - 1; i >= 0; i--)
        {
            std::cout << out[i];
        }
        std::cout << " ";
    }

    void printBit(uint32_t num, int len)
    {
        char out[100] = "";
        for (int i = 0; i < len; i++, num >>= 1)
        {
            if (num & 1)
                std::strcat(out, "1");
            else
                std::strcat(out, "0");
        }
        for (int i = len - 1; i >= 0; i--)
        {
            std::cout << out[i];
        }
        std::cout << " ";
    }

    void printBit(uint16_t num, int len)
    {
        char out[100] = "";
        for (int i = 0; i < len; i++, num >>= 1)
        {
            if (num & 1)
                std::strcat(out, "1");
            else
                std::strcat(out, "0");
        }
        for (int i = len - 1; i >= 0; i--)
        {
            std::cout << out[i];
        }
        std::cout << " ";
    }

    std::string BitString(uint64_t num, uint32_t len)
    {
        std::string res = "";
        for (int i = 0; i < len; i++, num >>= 1)
        {
            if (num & 1)
                res = "1" + res;
            else
                res = "0" + res;
        }
        return res;
    }

    std::string BitString(uint32_t num, uint32_t len)
    {
        std::string res = "";
        for (int i = 0; i < len; i++, num >>= 1)
        {
            if (num & 1)
                res = "1" + res;
            else
                res = "0" + res;
        }
        return res;
    }

    std::string BitString(uint16_t num, uint32_t len)
    {
        std::string res = "";
        for (int i = 0; i < len; i++, num >>= 1)
        {
            if (num & 1)
                res = "1" + res;
            else
                res = "0" + res;
        }
        return res;
    }

    std::string BitStringWithSpace(uint64_t num, uint32_t len)
    {
        std::string res = "";
        for (int i = 0; i < len; i++, num >>= 1)
        {
            if (i % 4 == 0)
                res = " " + res;
            if (num & 1)
                res = "1" + res;
            else
                res = "0" + res;
        }
        return res;
    }

    /* rounding */
    int64_t RoundToNearestEven(int64_t num, int point)
    {
        int64_t t = (int64_t)1 << point;

        bool lastBit = (bool)(num & t);
        t >>= 1;
        bool groundBit = (bool)(num & t);
        t >>= 1;
        bool roundBit = (bool)(num & t);
        t -= 1;
        bool stickyBits = (bool)(num & t);

        int64_t lsb = (int64_t)1 << point;
        if (groundBit)
        {
            if (!roundBit && !stickyBits)
            { // round to even
                if (lastBit)
                    num += lsb;
            }
            else
                num += lsb; // round up
        }

        return num;
    }
}

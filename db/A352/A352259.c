/*
 * A352259  Number of ways to write n as w^6 + x^2 + 2*y^2 + 3*z^2 + x*y*z,
 *          where w,x,y,z are nonnegative integers.
 * https://oeis.org/A352259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write n as w^6 + x^2 + 2*y^2 + 3*z^2 +
 *            x*y*z, where w,x,y,z are nonnegative integers
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t isqrt_uint64(uint64_t x)
{
    uint64_t r = 0;
    for (int k = 63; k >= 0; --k)
    {
        uint64_t bit = 1ULL << k;
        uint64_t trial = r | bit;
        if ((__uint128_t)trial * trial <= x)
            r = trial;
    }
    return r;
}

seq_t a(idx_t n)
{
    uint64_t n64 = (uint64_t)n;
    seq_t count = 0;

    /* iterate over w */
    for (uint64_t w = 0; ; ++w)
    {
        uint64_t wpow = 1;
        for (uint64_t i = 0; i < 6; ++i)
            wpow *= w;
        if (wpow > n64)
            break;

        uint64_t n1 = n64 - wpow;

        /* maximum possible x */
        uint64_t max_x = isqrt_uint64(n1);
        for (uint64_t x = 0; x <= max_x; ++x)
        {
            uint64_t x2 = x * x;
            if (x2 > n1)
                break;
            uint64_t s = n1 - x2;

            /* y is bounded by 2*y^2 <= s */
            uint64_t max_y = isqrt_uint64(s / 2);
            for (uint64_t y = 0; y <= max_y; ++y)
            {
                uint64_t yy = y * y;
                uint64_t t = s - 2 * yy;          /* remaining part */
                uint64_t xy = x * y;

                /* discriminant D = (x*y)^2 + 12*t */
                uint64_t d = xy * xy + 12 * t;
                uint64_t sqrtD = isqrt_uint64(d);
                if (sqrtD * sqrtD != d)
                    continue;
                if (sqrtD < xy)
                    continue;

                uint64_t numer = sqrtD - xy;
                if (numer % 6 != 0)
                    continue;
                uint64_t z = numer / 6;

                /* verify the equation */
                if (3 * z * z + xy * z == t)
                    count += 1;
            }
        }
    }
    return count;
}

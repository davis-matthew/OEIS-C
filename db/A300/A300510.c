/*
 * A300510  Number of ways to write n^2 as 4^k*(m^2+1) + x^2 + y^2, where m is
 *          1 or 2, and k,x,y are nonnegative integers with x <= y.
 * https://oeis.org/A300510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of ways to write n^2 as 4^k*(m^2+1) + x^2 + y^2,
   where m∈{1,2}, k,x,y≥0 and x ≤ y. */
#include <stdint.h>

static unsigned long long isqrt_u64(unsigned long long x)
{
    unsigned long long lo = 0, hi = 1ULL << 32;          /* hi^2 ≥ x for x ≤ 2^64 */
    while (lo <= hi)
    {
        unsigned long long mid = (lo + hi) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == x) return mid;
        if (sq < x)
            lo = mid + 1;
        else
        {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return lo - 1;                                       /* floor(sqrt(x)) */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* offset 1, a(1) is the first term */

    unsigned long long N   = (unsigned long long)n;
    unsigned long long n_sq = N * N;       /* n^2 */
    unsigned long long count = 0;

    for (int m = 1; m <= 2; ++m)            /* m = 1 or 2 */
    {
        unsigned long long base = (unsigned long long)(m * m + 1);   /* m^2 + 1 */
        unsigned long long pow4 = 1;                     /* 4^k, start with k = 0 */

        for (;;)
        {
            if (pow4 > n_sq / base) break;               /* 4^k * base would exceed n_sq */

            unsigned long long R = n_sq - pow4 * base;   /* remainder to be expressed as x^2 + y^2 */
            unsigned long long root = isqrt_u64(R);

            for (unsigned long long x = 0; x <= root; ++x)
            {
                unsigned long long x2 = x * x;
                unsigned long long y2 = R - x2;
                unsigned long long y  = isqrt_u64(y2);

                if (y * y == y2 && y >= x)
                    ++count;                  /* valid (x,y) pair */
            }

            if (pow4 > n_sq / (base * 4)) break;         /* next 4^k would overflow the limit */
            pow4 *= 4;                                 /* move to next k (4^(k+1)) */
        }
    }

    return (seq_t)count;
}

/*
 * A262816  Number of ordered ways to write n as x^3 + y^2 + z*(3*z-1)/2,
 *          where x and y are nonnegative integers, and z is a nonzero
 *          integer.
 * https://oeis.org/A262816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered triples (x,y,z) with x≥0, y≥0, z≠0
 *            integer such that n = x³ + y² + z*(3*z‑1)/2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Integer square root for 64‑bit unsigned integers. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid * mid <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t x = 0; ((idx_t)x) * ((idx_t)x) * ((idx_t)x) <= n; ++x)
    {
        idx_t x3 = ((idx_t)x) * ((idx_t)x) * ((idx_t)x);
        idx_t rem1 = n - x3;
        for (idx_t y = 0; ((idx_t)y) * ((idx_t)y) <= rem1; ++y)
        {
            idx_t y2 = ((idx_t)y) * ((idx_t)y);
            idx_t R = rem1 - y2;
            idx_t d = 1 + 24 * R;
            idx_t s = isqrt_u64(d);
            if (s * s != d)
                continue;

            /* Positive root */
            if ((1 + s) % 6 == 0)
            {
                idx_t z = (1 + s) / 6;
                if (z != 0)
                    ++count;
            }

            /* Negative root */
            if ((1 - (long long)s) % 6 == 0)
            {
                long long z = (1 - (long long)s) / 6;
                if (z != 0)
                    ++count;
            }
        }
    }
    return count;
}

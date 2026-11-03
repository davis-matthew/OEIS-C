/*
 * A275678  Number of ordered ways to write n as 4^k*(1+4*x^2+y^2) + z^2,
 *          where k,x,y,z are nonnegative integers with x <= y.
 * https://oeis.org/A275678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered tuples (k,x,y,z) of nonnegative
 *            integers with x ≤ y that satisfy n = 4^k * (1 + 4*x^2 + y^2) +
 *            z^2 . The function counts all such tuples
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9312 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n < 1, but return 0 */

    seq_t count = 0;
    seq_t pow4 = 1;          /* 4^k */

    while (pow4 <= n)
    {
        seq_t max_inner = n / pow4;   /* 1 + 4*x^2 + y^2 ≤ max_inner */

        for (idx_t x = 0; 1 + 4*x*x <= max_inner; ++x)
        {
            seq_t valx = 1 + 4*(seq_t)x*(seq_t)x;   /* 1 + 4*x^2 */

            for (idx_t y = x; valx + (seq_t)y*(seq_t)y <= max_inner; ++y)
            {
                seq_t inner = valx + (seq_t)y*(seq_t)y;   /* 1 + 4*x^2 + y^2 */
                seq_t t = pow4 * inner;                  /* 4^k * inner */
                seq_t rem = n - t;                       /* z^2 */

                seq_t z = floor_sqrt(rem);
                if (z*z == rem)
                    ++count;
            }
        }

        pow4 <<= 2;   /* next power of 4 */
    }

    return count;
}

/*
 * A344058  Number of ways to write n as x + y + z with x*y + 2*y*z + 3*z*x a
 *          square, where x,y,z are positive integers with x or y a power of
 *          two (including 2^0 = 1).
 * https://oeis.org/A344058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triples (x,y,z) of positive integers with * x +
 *            y + z = n, * x*y + 2*y*z + 3*z*x is a perfect square, * and at
 *            least one of x or y is a power of two
 * Range:     u8 n <= 2000, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   is_pow2_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Order of calls: arbitrary, no state needed.                     */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_pow2_u64.h"

/*--- helper: integer square root for 64‑bit values --------------------*/
static inline idx_t isqrt_u64(idx_t v)
{
    /* binary search for floor(sqrt(v)) */
    idx_t lo = 0, hi = v;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == v) return mid;
        if (sq < v) lo = mid + 1;
        else        hi = mid - 1;
    }
    return hi;   /* floor sqrt */
}

/*--- main function ---------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 3) return 0;          /* no positive integer triples possible */

    seq_t count = 0;

    /* Iterate over x and y.  z is determined by n - x - y. */
    for (idx_t x = 1; x <= n - 2; ++x) {
        bool xpow2 = is_pow2_u64(x);
        for (idx_t y = 1; y <= n - x - 1; ++y) {
            /* must have at least one of x or y a power of two */
            if (!xpow2 && !is_pow2_u64(y))
                continue;

            idx_t z = n - x - y;
            /* All x,y,z are positive by loop bounds */
            idx_t s = x * y + 2 * y * z + 3 * z * x;

            idx_t r = isqrt_u64(s);
            if (r * r == s)
                ++count;
        }
    }

    return count;
}

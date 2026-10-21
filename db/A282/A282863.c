/*
 * A282863  Number of ways to write n as x^2 + y^2 + z^2 + w^2 with x,y,z,w
 *          nonnegative integers such that 72*x^3 + (y-z)^3 is an even square.
 * https://oeis.org/A282863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts the ordered quadruples (x,y,z,w) of nonnegative
 *            integers * satisfying x^2 + y^2 + z^2 + w^2 = n and 72*x^3 + (y
 *            - z)^3 is an even * perfect square. This implementation performs
 *            a direct search over all * admissible x, y, z, computing w from
 *            the residual square. * It is efficient for n...
 * Range:     u8 n <= 505, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square‑root for 64‑bit values using binary search.
 * For the small values used here the cast to unsigned __int128 is safe. */
static inline idx_t isqrt_u64(idx_t v)
{
    idx_t lo = 0, hi = v;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if ((unsigned __int128)mid * mid <= (unsigned __int128)v)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t count = 0;

    /* Upper bound for x: floor(sqrt(n)) */
    idx_t x_max = isqrt_u64(n);

    for (idx_t x = 0; x <= x_max; ++x) {
        seq_t xsq = (seq_t)x * x;
        seq_t rem1 = (seq_t)n - xsq;
        /* rem1 fits in idx_t for our bounds */
        idx_t rem1_u = (idx_t)rem1;
        idx_t y_max = isqrt_u64(rem1_u);

        for (idx_t y = 0; y <= y_max; ++y) {
            seq_t ysq = (seq_t)y * y;
            seq_t rem2 = rem1 - ysq;
            idx_t rem2_u = (idx_t)rem2;
            idx_t z_max = isqrt_u64(rem2_u);

            for (idx_t z = 0; z <= z_max; ++z) {
                seq_t zsq = (seq_t)z * z;
                seq_t w2 = rem2 - zsq;          /* w^2, nonnegative */

                /* Check that w2 is a perfect square */
                idx_t w2_u = (idx_t)w2;
                idx_t ws = isqrt_u64(w2_u);
                if ((seq_t)ws * ws != w2)
                    continue;                 /* not a square, skip */

                /* Compute 72*x^3 + (y-z)^3 */
                int64_t x3 = (int64_t)x * x * x;
                int64_t diff = (int64_t)y - (int64_t)z;
                int64_t diff3 = diff * diff * diff;
                int64_t val = 72LL * x3 + diff3;

                if (val < 0)                /* cannot be a square */
                    continue;

                /* Check that val is an even square */
                idx_t val_u = (idx_t)val;
                idx_t t = isqrt_u64(val_u);
                if ((seq_t)t * t == val && (t & 1ULL) == 0)
                    ++count;
            }
        }
    }
    return count;
}

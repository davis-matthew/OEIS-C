/*
 * A303338  Number of ways to write n as x^2 + 2*y^2 + 3*2^z + 4^w with
 *          x,y,z,w nonnegative integers.
 * https://oeis.org/A303338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations of n as * n = x² + 2·y² + 3·2ᶻ
 *            + 4ʷ with x, y, z, w ≥ 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Integer square root of an unsigned 64‑bit value.
 * Uses binary search and no floating point operations. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid <= n / mid)   /* mid² ≤ n  – avoided overflow */
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Count solutions of r = x² + 2·y²  with x, y ≥ 0.
 * r fits in idx_t. */
static seq_t count_xy(idx_t r)
{
    seq_t cnt = 0;
    for (idx_t x = 0; ; ++x) {
        idx_t xsq = x * x;
        if (xsq > r)
            break;
        idx_t rem = r - xsq;
        if (rem & 1)            /* must be even for 2·y² */
            continue;
        idx_t y2 = rem >> 1;   /* rem / 2 */
        idx_t y  = isqrt_u64(y2);
        if (y * y == y2)
            ++cnt;
    }
    return cnt;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t sum = 0;

    /* Outer loop over z: 3·2ᶻ ≤ n → 2ᶻ ≤ n/3 */
    for (idx_t pow2z = 1; pow2z <= n / 3; pow2z <<= 1) {
        idx_t term_z = 3 * pow2z;              /* 3·2ᶻ */
        idx_t rem_after_z = n - term_z;

        /* Inner loop over w: 4ʷ ≤ rem_after_z */
        for (idx_t pow4w = 1; ; pow4w *= 4) {
            if (pow4w > rem_after_z)
                break;
            idx_t rem = rem_after_z - pow4w;
            sum += (idx_t)count_xy(rem);

            /* Prevent overflow of the next 4ʷ multiplication */
            if (pow4w > rem_after_z / 4)
                break;
        }
    }

    return (seq_t)sum;
}

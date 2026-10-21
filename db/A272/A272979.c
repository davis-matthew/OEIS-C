/*
 * A272979  Number of ways to write n as x^2 + 2*y^2 + 3*z^3 + 4*w^4 with
 *          x,y,z,w nonnegative integers.
 * https://oeis.org/A272979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count solutions to x^2 + 2*y^2 + 3*z^3 + 4*w^4 = n with x,y,z,w ≥ 0
   Implements: a(n) = #{(x,y,z,w) ∈ ℕ^4 : x^2 + 2y^2 + 3z^3 + 4w^4 = n} */
#include <stdbool.h>

/* Integer floor square‑root of a 64‑bit unsigned value.
   Uses binary search and 128‑bit arithmetic to avoid overflow. */
static inline idx_t _isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n, mid;
    while (lo <= hi) {
        mid = (lo + hi) >> 1;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;
}

seq_t a(idx_t n)
{
    seq_t count = 0;

    for (idx_t w = 0; ; ++w) {
        /* 4 * w^4 */
        idx_t w2 = w * w;          /* w²   */
        idx_t w4 = w2 * w2;        /* w⁴   */
        w4 *= 4;                   /* 4w⁴  */
        if (w4 > n) break;

        for (idx_t z = 0; ; ++z) {
            idx_t z3 = z * z * z;          /* z³   */
            idx_t val_z = 3 * z3;          /* 3z³  */
            idx_t sum_z = w4 + val_z;
            if (sum_z > n) break;

            for (idx_t y = 0; ; ++y) {
                idx_t y2 = y * y;          /* y²   */
                idx_t val_y = 2 * y2;      /* 2y²  */
                idx_t sum_y = sum_z + val_y;
                if (sum_y > n) break;

                idx_t rem = n - sum_y;     /* remaining for x² */
                idx_t x = _isqrt_u64(rem); /* floor sqrt of rem */
                if (x * x == rem)          /* perfect square?  */
                    ++count;
            }
        }
    }
    return count;
}

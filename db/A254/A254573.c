/*
 * A254573  Number of ways to write n = x*(x+1) + y*(3*y+1)/2 + z*(3*z-1)/2
 *          with x,y,z nonnegative integers.
 * https://oeis.org/A254573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of solutions (x,y,z) ∈ ℕ³ of n = x*(x+1) + y*(3*y+1)/2
 *            + z*(3*z-1)/2 (x,y,z are non‑negative integers)
 * Range:     u8 n <= 2, u16 n <= 358, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 8898 (2026-11-01)
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
    seq_t target = (seq_t)n;
    seq_t count  = 0;

    for (idx_t x = 0; ; ++x) {
        seq_t termX = (seq_t)x * ((seq_t)x + 1);   /* x*(x+1) */
        if (termX > target) break;
        seq_t remX = target - termX;

        for (idx_t y = 0; ; ++y) {
            seq_t termY = (seq_t)y * (((seq_t)3 * y) + 1) / 2; /* y*(3*y+1)/2 */
            if (termY > remX) break;
            seq_t remY = remX - termY;

            if (remY == 0) {            /* z = 0 works */
                ++count;
                continue;
            }

            seq_t D      = 1 + (seq_t)24 * remY;   /* discriminant 1+24*rem */
            seq_t sqrtD  = floor_sqrt(D);
            if (sqrtD * sqrtD != D) continue;     /* not a perfect square */

            /* z = (1 + sqrtD)/6 must be integer */
            if (((sqrtD + (seq_t)1) % 6) == 0) {
                ++count;
            }
        }
    }
    return count;
}

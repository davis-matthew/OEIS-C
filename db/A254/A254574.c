/*
 * A254574  Number of ways to write n = x*(x+1)/2 + y*(3*y+1)/2 + z*(3*z-1)/2
 *          with x,y,z nonnegative integers.
 * https://oeis.org/A254574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triples (x,y,z) of non‑negative integers
 *            satisfying n = x*(x+1)/2 + y*(3*y+1)/2 + z*(3*z-1)/2
 * Range:     u8 n <= 2, u16 n <= 70, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 8665 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    seq_t count = 0;
    seq_t N = (seq_t)n;

    for (idx_t x = 0; ; ++x) {
        /* triangular number T(x) = x*(x+1)/2 */
        seq_t Tx = (seq_t)x * ((seq_t)x + 1) / 2;
        if (Tx > N) break;
        seq_t rem1 = N - Tx;

        for (idx_t y = 0; ; ++y) {
            /* generalized pentagonal number P(y) = y*(3*y+1)/2 */
            seq_t Ay = (seq_t)y * ((seq_t)3 * (seq_t)y + 1) / 2;
            if (Ay > rem1) break;
            seq_t rem2 = rem1 - Ay;

            /* z must satisfy B(z) = z*(3*z-1)/2 == rem2 */
            if (rem2 == 0) {          /* z = 0 */
                ++count;
                continue;
            }

            /* Solve 3*z^2 - z - 2*rem2 = 0 */
            /* discriminant D = 1 + 24*rem2 must be a perfect square */
            seq_t D = 1 + 24 * rem2;
            seq_t sqrtD = floor_sqrt(D);
            if (sqrtD * sqrtD != D) continue;
            /* (1 + sqrtD) must be divisible by 6 for integer z */
            if (((seq_t)1 + sqrtD) % 6 == 0) {
                ++count;
            }
        }
    }
    return count;
}

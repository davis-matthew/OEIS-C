/*
 * A389657  a(n) is zero if n is a square, otherwise a(n) is the least
 *          triangular number m > 0 such that m*n is a triangular number.
 * https://oeis.org/A389657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n is a square; otherwise a(n) = (x² – 1) / 8 where x
 *            is the smallest odd integer > 1 such that n·x² + 1 – n is a
 *            perfect square
 * Range:     u8 n <= 4, u16 n <= 23, u32 n <= 127, u64 n <= 348,
 *            u128 n <= 348, big any n
 * Validated: standard, pass, 76 terms through n = 314 (2026-11-01)
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
    seq_t N = (seq_t)n;

    /* Return 0 immediately if n is a perfect square. */
    seq_t s = floor_sqrt(N);
    if (s * s == N)
        return 0;

    /* Search for the smallest odd x > 1 that satisfies the Pell condition. */
    seq_t x = 3;                       /* smallest odd integer > 1 */
    while (1) {
        seq_t x2 = x * x;              /* x² */
        seq_t c = N * x2 + 1 - N;      /* n·x² + 1 – n  */
        seq_t sq = floor_sqrt(c);      /* floor(√c) */
        if (sq * sq == c) {            /* exact square? */
            return (x2 - 1) / 8;       /* m = (x² – 1)/8 */
        }
        x += 2;                        /* advance to next odd value */
    }
}

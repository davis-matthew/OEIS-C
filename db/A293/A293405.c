/*
 * A293405  The integer k that minimizes |k/n^2 - phi^2|, where phi =
 *          (1+sqrt(5))/2 (golden ratio).
 * https://oeis.org/A293405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((1 + 3*n^2 + n^2*sqrt(5)) / 2)
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 47, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 64 terms through n = 970 (2026-11-01)
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
    /* special case n=0 */
    if (n == 0) return 0;

    /* X = n^2 */
    seq_t X = (seq_t)n * (seq_t)n;

    /* D = 5 * X^2  (needed for floor_sqrt) */
    seq_t tmp = X * X;   /* X^2 */
    seq_t D   = tmp * 5; /* 5*X^2 */

    /* C = floor(sqrt(5*X^2)) = floor(X*sqrt(5)) */
    seq_t C = floor_sqrt(D);

    /* A = 1 + 3*X */
    seq_t A = 1 + (seq_t)3 * X;

    /* Choose B such that B + A is even.
       If (C + A) is odd we use B = C-1, otherwise B = C. */
    seq_t B = ((C + A) & 1) ? (C - 1) : C;

    /* k = (B + A) / 2 */
    seq_t k = (B + A) / 2;

    return k;
}

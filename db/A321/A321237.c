/*
 * A321237  Start with a square of dimension 1 X 1, and repeatedly append
 *          along the squares of the previous step squares with half their
 *          side length that do not overlap with any prior square; a(n) gives
 *          the number of squares appended at n-th step.
 * https://oeis.org/A321237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*2^n - 12 for n > 1, a(1) = 1 (OEIS A321237)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    seq_t t = pow2(n);      /* 2^n */
    t *= (seq_t)5;
    t -= (seq_t)12;
    return t;
}

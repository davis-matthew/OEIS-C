/*
 * A247620  Start with a single hexagon; at n-th generation add a hexagon at
 *          each expandable vertex; a(n) is the sum of all label values at
 *          n-th generation. (See comment for construction rules.)
 * https://oeis.org/A247620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 12*2^n - 6*n - 11 Equivalent to a(n) = -11 + 3*2^(n+2) -
 *            6*n Derived from A247620 definition
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 60, u128 n <= 124,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* 2^n as seq_t */
    seq_t p = pow2(n);
    /* 12*2^n */
    seq_t val = p * (seq_t)12;
    /* 6*n + 11 */
    seq_t sub = ((seq_t)n * (seq_t)6) + (seq_t)11;
    return val - sub;
}

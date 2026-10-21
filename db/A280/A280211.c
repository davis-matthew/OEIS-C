/*
 * A280211  a(n) = n*(2^(n^2)).
 * https://oeis.org/A280211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*2^(n^2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute a(n) = n * 2^(n^2) in seq_t. */
    idx_t exp = n * n;      /* exponent n^2 */
    seq_t p = pow2(exp);    /* 2^(n^2) */
    return (seq_t)n * p;    /* multiply by n   */
}

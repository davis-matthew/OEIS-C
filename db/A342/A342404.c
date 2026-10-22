/*
 * A342404  a(n) = binomial(n,2)*(2^(n-2) - n + 1).
 * https://oeis.org/A342404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n,2)*(2^(n-2) - n + 1)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 25, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;

    /* binomial(n,2) = n*(n-1)/2 */
    seq_t bin = (seq_t)n * ((seq_t)n - 1) / 2;

    /* 2^(n-2) */
    seq_t pow = pow2(n - 2);

    /* (2^(n-2) - n + 1) */
    seq_t tmp = pow - (seq_t)n + 1;

    return bin * tmp;
}

/*
 * A302761  Number of total dominating sets in the n-barbell graph.
 * https://oeis.org/A302761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^n - n)^2 - (2^n - 2*n) (OEIS A302761)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset begins at 1; guard unused index */

    /* 2^n as seq_t */
    seq_t two_pow_n = pow_uint(2, n);

    seq_t n_seq = (seq_t)n;
    seq_t alpha = two_pow_n - n_seq;                /* 2^n - n */
    seq_t alpha2 = alpha * alpha;                  /* (2^n - n)^2 */
    seq_t sub = two_pow_n - (n_seq + n_seq);        /* 2^n - 2n */

    return alpha2 - sub;                            /* final formula */
}

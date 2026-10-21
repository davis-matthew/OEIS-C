/*
 * A257448  a(n) = 13*(2^n - 1) - 3*n^2 - 9*n.
 * https://oeis.org/A257448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*(2^n - 1) - 3*n^2 - 9*n
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* OEIS offset: a(1) is the first term */
    seq_t n_seq = (seq_t)n;
    seq_t p2    = pow2(n);                     /* 2^n */
    seq_t part  = p2 - (seq_t)1;               /* 2^n - 1 */
    seq_t prod  = (seq_t)13 * part;            /* 13*(2^n-1) */
    seq_t sub1  = (seq_t)3 * n_seq * n_seq;    /* 3*n^2 */
    seq_t sub2  = (seq_t)9 * n_seq;            /* 9*n */
    return prod - sub1 - sub2;
}

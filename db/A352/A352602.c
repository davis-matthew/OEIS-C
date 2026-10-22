/*
 * A352602  a(n) = 4^n*(2^(2*n+1)-1)*(2*n)!.
 * https://oeis.org/A352602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^n * (2^(2*n+1)-1) * (2*n)!
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   factorial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed directly with integer arithmetic. */

#include "../utils/pow_seq.h"
#include "../utils/factorial.h"

/* Computes the n-th term of sequence A352602. */
seq_t a(idx_t n)
{
    /* 4^n */
    seq_t p = pow_seq((seq_t)4, (int)n);

    /* 2^(2*n+1)-1 = 2*4^n-1 */
    seq_t term = (seq_t)2 * p - (seq_t)1;

    /* (2*n)!  */
    seq_t fac = factorial((idx_t)(2 * n));

    /* Final product */
    return p * term * fac;
}

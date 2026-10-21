/*
 * A241032  Sum of n-th powers of divisors of 30.
 * https://oeis.org/A241032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1+2^n)*(1+3^n)*(1+5^n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"   /* big_pow is declared elsewhere */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;          /* convert index to seq_t */
    seq_t p2 = big_pow((seq_t)2, n_seq);
    seq_t p3 = big_pow((seq_t)3, n_seq);
    seq_t p5 = big_pow((seq_t)5, n_seq);
    return (p2 + (seq_t)1) * (p3 + (seq_t)1) * (p5 + (seq_t)1);
}

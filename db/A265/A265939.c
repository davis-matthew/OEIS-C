/*
 * A265939  Central terms of triangle A102363.
 * https://oeis.org/A265939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*4^n - binomial(2*n, n))/2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* 4^n = 2^(2n) */
    seq_t pow4_n   = pow2(2 * n);
    /* 3*4^n */
    seq_t three_mul = pow4_n * (seq_t)3;
    /* binomial(2n, n) */
    seq_t bin  = binomial(2 * n, n);
    /* (3*4^n - binomial(2n,n)) / 2 */
    return (three_mul - bin) / (seq_t)2;
}

/*
 * A306609  a(n) = Sum_{k=0..n} k*binomial(4*n+2,2*k).
 * https://oeis.org/A306609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n+1)*(16^n - binomial(4*n,2*n))/2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* 16^n = 2^(4*n) */
    seq_t pow16 = pow2((idx_t)(4 * n));

    /* binomial(4*n, 2*n) */
    seq_t bin = binom((idx_t)(4 * n), (idx_t)(2 * n));

    /* (16^n - binomial) / 2 */
    seq_t diff = pow16 - bin;
    seq_t half = diff / (seq_t)2;

    /* (2*n + 1) * half */
    seq_t coef = (seq_t)(2 * n + 1);
    return coef * half;
}

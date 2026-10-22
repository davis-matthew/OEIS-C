/*
 * A383853  a(n) = Sum_{k=0..n} binomial(2*n, k) * (n-k)^(4*n).
 * https://oeis.org/A383853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(2*n, k) * (n-k)^(4*n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>            /* for standard integer types, if needed */
#include "../utils/big_pow.h"  /* prototype for big_pow(seq_t, seq_t) */

seq_t a(idx_t n)
{
    /* 0^0 is defined as 1, so a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    seq_t sum   = (seq_t)0;
    seq_t binom = (seq_t)1;                 /* binomial(2*n, 0) */
    seq_t two_n = (seq_t)2 * (seq_t)n;       /* 2*n as seq_t   */
    seq_t exp   = (seq_t)4 * (seq_t)n;       /* exponent 4*n    */

    for (idx_t k = (idx_t)0; k <= n; ++k)
    {
        seq_t base = (seq_t)n - (seq_t)k;    /* (n-k)  */
        seq_t p    = big_pow(base, exp);    /* (n-k)^(4*n)  */

        sum += binom * p;                   /* accumulate term */

        if (k == n)
            break;                          /* finished the loop */

        /* next binomial coefficient: binom(2*n, k+1) */
        binom = binom * (two_n - (seq_t)k) / ((seq_t)k + (seq_t)1);
    }
    return sum;
}

/*
 * A360586  Expansion of e.g.f.: exp(x)*(exp(x)-1)*(exp(x)-x).
 * https://oeis.org/A360586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n - 2^n - n*2^(n-1) + n
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* compute powers of 2 and 3 using the supplied big‑power routine */
    seq_t pow3     = big_pow((seq_t)3, (seq_t)n);        /* 3^n   */
    seq_t pow2n    = big_pow((seq_t)2, (seq_t)n);        /* 2^n   */
    seq_t pow2n_1  = big_pow((seq_t)2, (seq_t)(n - 1));  /* 2^(n-1) */

    seq_t result = pow3 - pow2n;                        /* 3^n - 2^n */
    result -= (seq_t)n * pow2n_1;                       /* - n*2^(n-1) */
    result += (seq_t)n;                                 /* + n */

    return result;
}

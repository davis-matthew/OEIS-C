/*
 * A353042  G.f. A(x) satisfies: A(x) = 1 + x * A(4*x/(1 + 3*x)) / (1 - x).
 * https://oeis.org/A353042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n-1,k-1) * 2^(k*(k-1)) (a(0)=1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the terms of OEIS sequence A353042. */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the offset: a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    seq_t result = 0;
    seq_t cur_binom = 1;   /* binomial(n-1,0) */
    seq_t pow2_val = 1;    /* 2^(1*0) = 1 */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* Add the current term */
        result += cur_binom * pow2_val;

        /* No need to update after the last term */
        if (k == n)
            break;

        /* Update the power of 2: exponent increases by 2*k each step */
        seq_t shift = pow2((idx_t)(2 * k));   /* 2^(2*k) */
        pow2_val = pow2_val * shift;

        /* Update the binomial coefficient: */
        /* binomial(n-1,k) = binomial(n-1,k-1) * (n-k) / k */
        cur_binom = (cur_binom * (seq_t)(n - k)) / (seq_t)k;
    }

    return result;
}

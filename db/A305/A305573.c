/*
 * A305573  Number of (1,1) pairs occurring at depth 3n of the Fibonacci tree.
 * https://oeis.org/A305573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*Sum_{k=0..n} (k + 1)^2 * C(3*n + 2, n - k) / (2*n + k +
 *            3)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   binomial_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial_value.h"
/* The binary coefficients are computed multiplicatively to avoid factorial overflow. */

seq_t a(idx_t n)
{
    idx_t N = 3 * n + 2;               /* the binomial upper index */
    seq_t bin = binomial_value(N, n); /* start with C(N, n)  */
    seq_t result = 0;
    idx_t r = n;                       /* current lower index in the binomial */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t denom = 2 * n + k + 3;      /* denominator of the term */

        seq_t term = bin;                /* start with the current binomial */
        seq_t kplus1 = (seq_t)(k + 1);
        seq_t sq = kplus1 * kplus1;      /* (k+1)^2  */
        term *= sq;                     /* multiply by (k+1)^2   */
        term *= 3;                      /* multiply by outer 3   */
        term /= denom;                  /* integer division  */

        result += term;                 /* accumulate */

        if (k < n) {                    /* prepare binomial for next k */
            seq_t tmp = bin;
            tmp *= (seq_t)r;            /* multiply by r */
            seq_t div = N - r + 1;      /* divide by (N - r + 1) */
            tmp /= div;
            bin = tmp;
            r--;                        /* decrement lower index */
        }
    }
    return result;
}

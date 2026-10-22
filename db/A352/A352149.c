/*
 * A352149  a(n) = Sum_{k=0..n-1} binomial(n,k)^2 * (n-k-1)!.
 * https://oeis.org/A352149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} binomial(n,k)^2 * (n-k-1)!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* a(0) == 0 by definition */
    if (n == 0)
        return (seq_t)0;

    /* Pre‑compute factorials 0! … n!  (we will use up to (n-1)! ) */
    seq_t *fact = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!fact)                /* allocation failure – return 0 as a safe fallback */
        return (seq_t)0;

    fact[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;
    }

    seq_t sum   = (seq_t)0;
    seq_t binom = (seq_t)1;            /* C(n,0) */

    for (idx_t k = 0; k < n; ++k) {
        /* term = C(n,k)^2 * (n-k-1)! */
        seq_t term = binom * binom * fact[n - k - 1];
        sum += term;

        /* Update binomial coefficient to C(n,k+1) for next iteration */
        if (k + 1 < n) {
            seq_t numerator   = (seq_t)(n - k);
            seq_t denominator = (seq_t)(k + 1);
            binom = (binom * numerator) / denominator;   /* exact division */
        }
    }

    free(fact);
    return sum;
}

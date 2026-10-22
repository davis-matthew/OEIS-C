/*
 * A344097  Expansion of Product_{k>=1} (1 + x^k)^binomial(k+4,4).
 * https://oeis.org/A344097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Sum_{k=1..n} ( Sum_{d|k} (-1)^(k/d+1) *
 *            A174002(d) ) * a(n-k) with a(0)=1. A174002(d) = d *
 *            binomial(d+4,4)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 14, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binom_dplus4_4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom_dplus4_4.h"

/* Main sequence member function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Pre‑compute A174002(d) = d * binom(d+4,4) for 1..n. */
    seq_t *A = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!A)
        return (seq_t)0;
    for (idx_t d = 1; d <= n; ++d)
        A[d] = (seq_t)d * binom_dplus4_4(d);

    /* Compute S[1..n] = sum_{d|k} (-1)^(k/d+1) * A[d] (non‑negative). */
    seq_t *S = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!S) {
        free(A);
        return (seq_t)0;
    }
    for (idx_t k = 1; k <= n; ++k) {
        seq_t pos = 0;
        seq_t neg = 0;
        for (idx_t d = 1; d * d <= k; ++d) {
            if (k % d == 0) {
                /* divisor d */
                seq_t term = A[d];
                idx_t exp = k / d;          /* k/d */
                if ((exp & 1) == 0)         /* even exp -> sign = -1 */
                    neg += term;
                else
                    pos += term;

                /* divisor k/d if distinct */
                idx_t d2 = k / d;
                if (d2 != d) {
                    term = A[d2];
                    exp = k / d2;           /* which is d */
                    if ((exp & 1) == 0)
                        neg += term;
                    else
                        pos += term;
                }
            }
        }
        /* pos ≥ neg, so subtraction is safe */
        S[k] = pos - neg;
    }
    free(A);

    /* Compute the sequence via the recurrence. */
    seq_t *seq = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!seq) {
        free(S);
        return (seq_t)0;
    }
    seq[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k)
            sum += S[k] * seq[i - k];
        seq[i] = sum / (seq_t)i;   /* division is exact */
    }

    seq_t result = seq[n];
    free(S);
    free(seq);
    return result;
}

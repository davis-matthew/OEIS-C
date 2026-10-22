/*
 * A376518  E.g.f. satisfies A(x) = exp(x^3 * (1 + x) * A(x)).
 * https://oeis.org/A376518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * Sum_{k=0..⌊n/3⌋} (k+1)^(k-1) * binomial(k, n-3k) /
 *            k!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   binomial_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A376518 by explicit summation. */

#include <stddef.h>  /* For size_t, not required but harmless */
#include "../utils/binomial_local.h"

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Main sequence function */
seq_t a(idx_t n)
{
    /* Compute n! */
    seq_t fact_n = 1;
    for (idx_t i = 1; i <= n; ++i) {
        fact_n *= (seq_t)i;
    }

    seq_t sum = 0;
    idx_t max_k = n / 3;
    seq_t fact_k = 1;  /* k! for the current k (starts with 0! = 1) */

    for (idx_t k = 0; k <= max_k; ++k) {
        idx_t r = n - 3 * k;
        if (r > k) {                 /* binomial(k, r) would be zero */
            /* skip this k */
        } else {
            seq_t binom = binomial_local(k, r);

            /* Compute (k+1)^(k-1).  For k==0 the convention is 1. */
            seq_t power = 1;
            if (k > 0) {
                seq_t base = (seq_t)(k + 1);
                idx_t exp = k - 1;
                for (idx_t e = 0; e < exp; ++e) {
                    power *= base;
                }
            }

            seq_t term = fact_n / fact_k;   /* n! / k! */
            term *= power;
            term *= binom;
            sum += term;
        }

        /* Prepare fact_k for the next iteration: (k+1)!  */
        fact_k *= (seq_t)(k + 1);
    }

    return sum;
}

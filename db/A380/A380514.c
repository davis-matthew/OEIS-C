/*
 * A380514  Expansion of e.g.f. exp(x*G(x)^2) where G(x) = 1 + x*G(x)^4 is the
 *          g.f. of A002293.
 * https://oeis.org/A380514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * n! * Sum_{k=0..n-1} binomial(2*n+2*k,k) / ((2*n+2*k)
 *            * (n-k-1)!) (n > 0), a(0) = 1 This implementation memoizes the
 *            sequence and factorials. It uses the available binomial(idx_t,
 *            idx_t) helper; all arithmetic is done with seq_t and idx_t only,
 *            as required
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 18,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/binomial.h"   /* binomial(idx_t, idx_t) */
#include <stdlib.h>              /* malloc, free */

seq_t a(idx_t n)
{
    /* Memoization tables */
    static seq_t *seq_vals   = NULL; /* a[0..max_n]     */
    static seq_t *fact_vals  = NULL; /* factorial[0..max_n] */
    static idx_t   max_n      = 0;    /* highest n computed so far */

    /* Initialise for n = 0 */
    if (seq_vals == NULL) {
        /* Allocate for a single element */
        seq_vals  = (seq_t *)malloc(sizeof(seq_t));
        fact_vals = (seq_t *)malloc(sizeof(seq_t));
        if (seq_vals == NULL || fact_vals == NULL) return (seq_t)0;
        seq_vals[0]  = (seq_t)1;         /* a(0) = 1 */
        fact_vals[0] = (seq_t)1;         /* 0! = 1 */
        max_n = 0;
    }

    if (n == 0) return seq_vals[0];

    /* If we need values beyond the cached range, grow the tables */
    if (n > max_n) {
        idx_t new_limit = n;
        size_t new_size = (size_t)(new_limit + 1);

        /* Allocate new tables */
        seq_t *new_seq_vals  = (seq_t *)malloc(new_size * sizeof(seq_t));
        seq_t *new_fact_vals = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (new_seq_vals == NULL || new_fact_vals == NULL) {
            /* Allocation failed – return 0 as a safe failure value */
            free(new_seq_vals);
            free(new_fact_vals);
            return (seq_t)0;
        }

        /* Copy old data to the new tables */
        for (idx_t i = 0; i <= max_n; ++i) {
            new_seq_vals[i]  = seq_vals[i];
            new_fact_vals[i] = fact_vals[i];
        }

        /* Compute new factorials */
        for (idx_t i = max_n + 1; i <= new_limit; ++i) {
            new_fact_vals[i] = new_fact_vals[i - 1] * (seq_t)i;
        }

        /* Compute sequence values for new indices */
        for (idx_t i = max_n + 1; i <= new_limit; ++i) {
            seq_t sum = (seq_t)0;
            seq_t fact_i = new_fact_vals[i];           /* i!      */
            for (idx_t k = 0; k <= i - 1; ++k) {
                idx_t two_n_2k = 2 * i + 2 * k;       /* 2*i+2*k */
                seq_t bin = binomial(two_n_2k, k);    /* binom(2*i+2*k, k) */
                /* numerator = 2 * i! * bin */
                seq_t numer = bin * fact_i * (seq_t)2;
                /* denominator = (2*i+2*k) * (i-k-1)! */
                seq_t denom = (seq_t)two_n_2k * new_fact_vals[i - k - 1];
                seq_t term   = numer / denom;        /* exact division */
                sum += term;
            }
            new_seq_vals[i] = sum;
        }

        /* Replace old tables */
        free(seq_vals);
        free(fact_vals);
        seq_vals  = new_seq_vals;
        fact_vals = new_fact_vals;
        max_n = new_limit;
    }

    return seq_vals[n];
}

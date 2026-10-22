/*
 * A300673  Expansion of e.g.f. exp(Sum_{k>=1} mu(k)*x^k/k!), where mu() is
 *          the Moebius function (A008683).
 * https://oeis.org/A300673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} mu(k) * C(n-1,k-1) * a(n-k) with a(0) = 1.
 *            Recurrence: a(n) = Sum_{k=1..n} mobius(k) * binomial(n-1,k-1) *
 *            a(n-k) (n>=1)
 * Range:     i8 n <= 7, i16 n <= 11, i32 n <= 17, i64 n <= 29, i128 n <= 48,
 *            big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mobius.h"

/* Static cache of computed terms */
static seq_t *a_cache = NULL;
static size_t a_cache_cap = 0;
static idx_t a_cache_size = 0;

/* Ensure the cache can hold indices up to n */
static void expand_a_cache(idx_t n)
{
    if ((size_t)(n + 1u) <= a_cache_cap)
        return;
    size_t new_cap = a_cache_cap ? a_cache_cap * 2u : 16u;
    while ((size_t)(n + 1u) > new_cap)
        new_cap <<= 1u;   /* double until large enough */
    seq_t *tmp = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    a_cache = tmp;
    a_cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Return cached value if already computed */
    if (n < a_cache_size)
        return a_cache[n];

    /* Grow cache if necessary */
    expand_a_cache(n);

    /* Initialize a(0) if cache is still empty */
    if (a_cache_size == 0) {
        a_cache[0] = 1;       /* a(0) = 1 */
        a_cache_size = 1;
    }

    /* Compute new values up to n */
    for (idx_t i = a_cache_size; i <= n; ++i) {
        seq_t sum = 0;
        seq_t binom = 1;     /* binomial(i-1,0) */

        for (idx_t k = 1; k <= i; ++k) {
            seq_t mu_k = mobius(k);   /* mu(k) */
            if (mu_k != 0) {
                seq_t term = a_cache[i - k];
                term = term * binom;
                term = term * mu_k;
                sum = sum + term;
            }
            /* update binomial coefficient for next k */
            if (k < i) {
                seq_t num = (seq_t)(i - k); /* (i-k) */
                seq_t den = (seq_t)k;       /* k */
                binom = (binom * num) / den;
            }
        }
        a_cache[i] = sum;
    }
    a_cache_size = n + 1;
    return a_cache[n];
}

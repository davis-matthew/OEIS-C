/*
 * A344037  Expansion of e.g.f.: exp(-2*x) / (2 - exp(x)).
 * https://oeis.org/A344037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n-1} binomial(n,k) * a(k) + (-2)^n (Recurrence
 *            derived from the e.g.f. exp(-2*x)/(2-exp(x))
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 11, i64 n <= 18, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    /* static cache of computed values */
    static seq_t *cache = NULL;        /* array of a(0) ... a(computed_upto)   */
    static size_t cache_cap = 0;       /* allocated capacity */
    static idx_t  computed_upto = 0;   /* highest index with a value computed   */
    static seq_t pow_neg2 = 1;         /* (-2)^k for the current k (k == computed_upto) */

    /* Ensure cache has room for up to n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0; /* out of memory, return 0 */
        if (cache) {
            memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
            free(cache);
        }
        /* zero initialize new entries */
        if (new_cap > cache_cap)
            memset(new_cache + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Initialize a(0) if not already */
    if (computed_upto == 0) {
        cache[0] = 1;          /* a(0) = 1 */
    }

    /* Compute values up to n lazily */
    for (idx_t i = computed_upto + 1; i <= n; ++i) {
        seq_t sum = 0;                /* accumulator for Sum_{k=0..i-1} binom(i,k)*a(k) */
        seq_t binom = 1;              /* binom(i,0) */

        for (idx_t k = 0; k < i; ++k) {
            seq_t prod = cache[k] * binom;
            sum += prod;

            /* Update binom to binom(i,k+1) for next iteration */
            if (k + 1 < i) {
                seq_t tmp = binom * (seq_t)(i - k);
                binom = tmp / (seq_t)(k + 1);
            }
        }

        /* (-2)^i = (-2)^(i-1) * (-2) */
        seq_t pow_neg2_i = pow_neg2 * (seq_t)(-2);
        cache[i] = sum + pow_neg2_i;

        /* Prepare pow_neg2 for next i */
        pow_neg2 = pow_neg2_i;
    }

    computed_upto = n;
    return cache[n];
}

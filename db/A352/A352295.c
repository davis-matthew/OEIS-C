/*
 * A352295  Expansion of e.g.f. 1/(exp(x) - x/(1 + x)).
 * https://oeis.org/A352295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} ((-1)^(k-1) * k! - 1) * binomial(n,k) *
 *            a(n-k) a(0) = 1 Recurrence implementation with memoisation
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are provided by the build system */
seq_t a(idx_t n)
{
    /* static cache for already computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* elements currently stored, cache[0..cache_len-1] */

    /* Initialise cache at first call, ensure a(0) is stored */
    if (cache_len == 0) {
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        cache[0] = (seq_t)1;   /* a(0) = 1 */
    }

    /* Grow the cache if the requested n is not yet available */
    if (n >= cache_len) {
        idx_t old_len = cache_len;
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (!tmp) { /* reallocation failed, return 0 as a safe fallback */
            return (seq_t)0;
        }
        cache = tmp;
        /* compute missing values from old_len to n inclusive */
        for (idx_t i = old_len; i <= n; ++i) {
            if (i == 0) {            /* already initialised */
                continue;
            }
            seq_t sum = (seq_t)0;
            seq_t binom = (seq_t)1;    /* C(i,0) */
            seq_t fact_k = (seq_t)1;   /* (k-1)! initially */
            for (idx_t k = 1; k <= i; ++k) {
                /* Update factorial: k! */
                fact_k *= (seq_t)k;

                /* Update binomial coefficient: C(i,k) from C(i,k-1) */
                binom = binom * (seq_t)(i - k + 1) / (seq_t)k;

                /* Compute ((-1)^(k-1) * k! - 1) */
                seq_t term_factor = fact_k;
                if ((k & 1) == 0)        /* k even => (-1)^(k-1) = -1 */
                    term_factor = -term_factor;
                term_factor -= (seq_t)1;

                /* Add term: binom * term_factor * a(i-k) */
                seq_t term = binom * term_factor * cache[i - k];
                sum += term;
            }
            cache[i] = sum;
        }
        cache_len = new_len;
    }

    return cache[n];
}

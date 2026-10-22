/*
 * A367853  Expansion of e.g.f. 1/(1 - x + log(1 - 4*x)/4).
 * https://oeis.org/A367853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A367853: Expansion of e.g.f. 1/(1 - x + log(1 - 4*x)/4)
   a(0) = 1
   a(n) = n*a(n-1) + Σ_{k=1..n} 4^(k-1)*(k-1)!*binomial(n,k)*a(n-k)   */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Persistent caches: a[n], 4^n, n!, and (k-1)!*4^(k-1) */
    static seq_t *cache_a    = NULL;
    static seq_t *cache_pow4 = NULL;
    static seq_t *cache_fact = NULL;
    static seq_t *cache_g    = NULL;
    static idx_t cache_size  = 0;   /* number of allocated elements */

    /* Grow the caches if needed */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;

        /* realloc returns void* – cast in both C and C++ */
        cache_a    = (seq_t*)realloc(cache_a,    new_size * sizeof(seq_t));
        cache_pow4 = (seq_t*)realloc(cache_pow4, new_size * sizeof(seq_t));
        cache_fact = (seq_t*)realloc(cache_fact, new_size * sizeof(seq_t));
        cache_g    = (seq_t*)realloc(cache_g,    new_size * sizeof(seq_t));

        /* Initialise new entries */
        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i == 0) {
                cache_pow4[0] = (seq_t)1;
                cache_fact[0] = (seq_t)1;
                cache_g[0]    = (seq_t)1; /* unused, but harmless */
                cache_a[0]    = (seq_t)1; /* a(0) = 1 */
            } else {
                cache_pow4[i] = cache_pow4[i-1] * (seq_t)4;
                cache_fact[i] = cache_fact[i-1] * (seq_t)i;
                cache_g[i]    = cache_fact[i-1] * cache_pow4[i-1]; /* (i-1)! * 4^(i-1) */
            }
        }
        cache_size = new_size;
    }

    /* Compute a[n] if not already done */
    if (n == 0) return cache_a[0];

    /* If a[n] is already computed (because of earlier calls), just return it */
    static idx_t computed_max = 0; /* largest n that has a[n] computed */
    if (n <= computed_max) return cache_a[n];

    /* Compute a[i] for i = computed_max+1 .. n */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        /* Sum over k=1..i */
        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)i;          /* C(i,1) = i */
        for (idx_t k = 1; k <= i; ++k) {
            if (k > 1) {
                /* binom(i,k) = binom(i,k-1) * (i-k+1) / k */
                binom = binom * (seq_t)(i - k + 1) / (seq_t)k;
            }
            /* term = g[k] * binom * a[i-k] */
            sum += cache_g[k] * binom * cache_a[i - k];
        }
        cache_a[i] = (seq_t)i * cache_a[i - 1] + sum;
    }
    computed_max = n;

    return cache_a[n];
}

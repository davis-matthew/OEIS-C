/*
 * A383314  Expansion of e.g.f. exp(-x/2) / (1-4*x)^(1/8).
 * https://oeis.org/A383314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=0, a(n)=2*(n-1)*(2*a(n-1)+a(n-2)) for n>1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache to store already computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static idx_t  cache_len = 0;   /* number of values computed so far */

    /* Ensure we have space for up to n (inclusive) */
    if (cache_cap < (size_t)(n + 1)) {
        size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 1;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Memory allocation failed. In this context we simply return 0. */
            return 0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize base cases if not already done */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }
    if (n >= 1 && cache_len < 2) {
        cache[1] = (seq_t)0;
        cache_len = 2;
    }

    /* If the requested value is already computed, return it */
    if (n < cache_len)
        return cache[n];

    /* Compute missing values incrementally using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t a_prev          = cache[i - 1];
        seq_t a_prevprev      = cache[i - 2];
        seq_t mult_factor     = 2 * ((seq_t)(i) - (seq_t)1);
        seq_t inner_sum       = 2 * a_prev + a_prevprev;
        cache[i] = mult_factor * inner_sum;
    }
    cache_len = n + 1;

    return cache[n];
}

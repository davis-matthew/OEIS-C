/*
 * A377960  Expansion of e.g.f. exp(x - x^2)/(1 - x)^3.
 * https://oeis.org/A377960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (n+3)*a(n-1) - 3*(n-1)*a(n-2) + 2*(n-1)*(n-2)*a(n-3)
 * Initial values: a(0) = 1, a(1) = 4, a(2) = 17
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t computed_max = (idx_t)-1;  /* no entries computed yet */

    /* initialise the cache lazily */
    if (cache == NULL) {
        /* ensure the cache can hold at least n+1 elements */
        cache_cap = (n > 4) ? (size_t)(n + 4) : 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)4;   /* a(1) */
        cache[2] = (seq_t)17;  /* a(2) */
        computed_max = 2;
    }

    /* grow the cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n >= new_cap)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    if (n > computed_max) {
        idx_t i;
        for (i = computed_max + 1; i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)4;
            } else if (i == 2) {
                cache[i] = (seq_t)17;
            } else {
                /* recurrence: a(i) = (i+3)*a(i-1) - 3*(i-1)*a(i-2) + 2*(i-1)*(i-2)*a(i-3) */
                seq_t term1 = ( (seq_t)(i + 3) ) * cache[i - 1];
                seq_t term2 = ( (seq_t)3 * (i - 1) ) * cache[i - 2];
                seq_t term3 = ( (seq_t)2 * (i - 1) * (i - 2) ) * cache[i - 3];
                cache[i] = term1 - term2 + term3;
            }
        }
        computed_max = n;
    }

    return cache[n];
}

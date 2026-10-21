/*
 * A255406  Expansion of the hyperbolic arc lemniscate tangent function.
 * https://oeis.org/A255406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{k=1}^{4n} (k - (k%4==0 ? 1 : 0))
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External definition of the sequence */
seq_t a(idx_t n)
{
    /* Cached values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Lazy allocation of the cache */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }

    /* Grow the cache if necessary and fill missing entries */
    if ((size_t)n >= cache_len) {
        /* New required size (n+1 elements) */
        size_t need = (size_t)n + 1;

        /* Grow capacity to at least 'need' */
        if (need > cache_cap) {
            size_t new_cap = cache_cap;
            while (new_cap < need)
                new_cap *= 2;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        /* Compute entries from cache_len up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;   /* a(0) = 1 */
            } else {
                seq_t val = cache[i - 1];
                idx_t start = 4 * (i - 1) + 1;
                idx_t end   = 4 * i;
                for (idx_t k = start; k <= end; ++k) {
                    seq_t term;
                    if (k % 4 == 0)
                        term = (seq_t)(k - 1);
                    else
                        term = (seq_t)k;
                    val = val * term;
                }
                cache[i] = val;
            }
        }
        cache_len = need;
    }

    return cache[n];
}

/*
 * A391372  a(n) = 2*a(n-1)^2 + a(n-1) + 1 with a(0) = 0.
 * https://oeis.org/A391372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)^2 + a(n-1) + 1, a(0) = 0
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;     /* cache of already computed terms */
    static idx_t cache_cap = 0;       /* allocated capacity of cache */
    static idx_t cache_len = 0;       /* number of terms stored (cache_len == last index + 1) */

    /* Allocate or grow the cache if necessary */
    if (n >= cache_cap) {
        idx_t new_cap = (cache_cap == 0 ? 1 : cache_cap);
        while (new_cap <= n) new_cap <<= 1;   /* double until we have room for n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                    /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n */
    if (n >= cache_len) {
        if (cache_len == 0) {         /* initialise a(0) */
            cache[0] = 0;
            cache_len = 1;
        }
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            cache[i] = 2 * prev * prev + prev + 1;
        }
        cache_len = n + 1;
    }

    return cache[n];
}

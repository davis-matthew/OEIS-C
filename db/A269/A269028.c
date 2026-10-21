/*
 * A269028  a(n) = 40*a(n - 1) - a(n - 2) for n>1, a(0) = 1, a(1) = 1.
 * https://oeis.org/A269028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 40*a(n - 1) - a(n - 2), a(0)=1, a(1)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* cache the already computed terms */
    static seq_t *cache   = NULL;   /* array of terms */
    static idx_t cache_last = 0;    /* last index that has a valid value */
    static size_t cache_cap   = 0;   /* allocated capacity (in terms) */
    static bool  initialized  = false;

    /* initialise cache on first call, at least 2 terms */
    if (!initialized) {
        cache_cap = (size_t)((n > 2) ? n + 1 : 3);
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache_last = 1;
        initialized = true;
    }
    /* grow the cache if needed */
    else if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failure – just return 0 (unlikely in tests) */
            return 0;
        }
        cache   = tmp;
        cache_cap = new_cap;
    }

    /* if already computed, return immediately */
    if (n <= cache_last) {
        return cache[n];
    }

    /* compute missing terms sequentially */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        cache[i] = (seq_t)40 * cache[i - 1] - cache[i - 2];
    }
    cache_last = n;
    return cache[n];
}

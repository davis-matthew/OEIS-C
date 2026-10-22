/*
 * A395029  Number of {0,1}-labelings of the ladder graph L_n that are
 *          unchanged when every vertex updates to the majority of itself and
 *          its neighbors, keeping its value on a tie.
 * https://oeis.org/A395029
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-2) - 2*a(n-4) for n >= 6 with a(2) = 6,
 *            a(3) = 22, a(4) = 68, a(5) = 222. Offset: 2
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Internal cache for the sequence values */
static seq_t *cache = NULL;
static idx_t cached_max = 0;          /* highest index computed so far */
static size_t cache_cap = 0;          /* allocated capacity of cache */

/* Ensure the cache is large enough for indices up to n */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        cache_cap = (n > 10 ? n + 1 : 16);
        if (cache_cap < 6) cache_cap = 6;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();          /* memory failure */
        for (idx_t i = 0; i < cache_cap; ++i) cache[i] = 0;
        /* initialise known values */
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)22;
        cache[4] = (seq_t)68;
        cache[5] = (seq_t)222;
        cached_max = 5;
        return;
    }

    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < (size_t)(n + 1)) new_cap = n + 1 + 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* memory failure */
        /* zero any newly allocated space */
        for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;            /* behaviour below offset is not important */

    ensure_cache(n);

    if (n <= cached_max) return cache[n];

    /* compute missing values up to n */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        if (i <= 5) continue;        /* already initialised during allocation */
        seq_t term = 3 * cache[i-1] + cache[i-2];
        term -= 2 * cache[i-4];
        cache[i] = term;
    }

    cached_max = n;
    return cache[n];
}

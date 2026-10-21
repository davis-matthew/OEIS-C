/*
 * A295690  a(n) = a(n-1) + a(n-3) + a(n-4), where a(0) = 2, a(1) = 2, a(2) =
 *          1, a(3) = 1.
 * https://oeis.org/A295690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-4), with a(0)=2, a(1)=2, a(2)=1,
 *            a(3)=1
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* for size_t */

/* Fixed maximum cache size – sufficient for the tests (n <= 10000). */
#define MAX_CACHE 200000

seq_t a(idx_t n)
{
    /* Static cache of computed terms. */
    static seq_t cache[MAX_CACHE];
    static size_t cache_len = 0;   /* number of terms that are valid in cache */

    /* First call – initialise the base values. */
    if (cache_len == 0) {
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)1;
        cache_len = 4;
    }

    /* If the requested term is already cached, return it. */
    if ((idx_t)cache_len > n) {
        return cache[n];
    }

    /* Ensure the request can fit in the cache.  The tests never exceed
       MAX_CACHE, but if a larger n is requested we fall back to a simple
       linear computation without expanding the cache. */
    if ((size_t)n + 1 > MAX_CACHE) {
        size_t i;
        seq_t *tmp = NULL;

        /* Allocate a temporary array large enough for the requested index. */
        tmp = (seq_t *)malloc(((size_t)n + 1) * sizeof(seq_t));
        if (tmp == NULL) {
            /* Allocation failed – return 0 as a sentinel. */
            return (seq_t)0;
        }

        /* Copy any previously cached values. */
        for (i = 0; i < cache_len; ++i) {
            tmp[i] = cache[i];
        }

        /* Compute the remaining terms. */
        for (i = cache_len; i <= (size_t)n; ++i) {
            tmp[i] = tmp[i - 1] + tmp[i - 3] + tmp[i - 4];
        }

        /* Copy back into the static cache if we are still within bounds
           (for future calls that fit in MAX_CACHE). */
        if (cache_len <= MAX_CACHE) {
            size_t limit = ((size_t)n + 1 < MAX_CACHE) ? ((size_t)n + 1) : MAX_CACHE;
            for (i = cache_len; i < limit; ++i) {
                cache[i] = tmp[i];
            }
            cache_len = limit;
        }

        seq_t result = tmp[(size_t)n];
        free(tmp);
        return result;
    }

    /* Compute missing terms up to n and extend cache. */
    size_t i;
    for (i = cache_len; i <= (size_t)n; ++i) {
        cache[i] = cache[i - 1] + cache[i - 3] + cache[i - 4];
    }
    cache_len = (size_t)n + 1;

    return cache[n];
}

/*
 * A261307  a(n+1) = abs(a(n) - gcd(a(n), 7*n+6)), a(1) = 1.
 * https://oeis.org/A261307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n ≥ 1: a(n+1) = |a(n) – gcd(a(n), 7*n+6)|
 * Range:     u8 n <= 73, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;         /* cache[1..cache_size] holds computed terms */
    static size_t cache_capacity = 0;   /* allocated size (excluding unused 0 slot)   */
    static idx_t  cache_size = 0;       /* last index computed                         */

    if (cache == NULL) {
        /* initial allocation */
        cache_capacity = 16;
        cache = (seq_t *)realloc(NULL, (cache_capacity + 1) * sizeof(seq_t));
        for (size_t i = 0; i <= cache_capacity; ++i)
            cache[i] = 0;
        cache[1] = (seq_t)1;
        cache_size = 1;
    }

    if (n <= cache_size)
        return cache[n];

    /* ensure enough storage for indices up to n */
    if ((size_t)n > cache_capacity) {
        size_t new_cap = cache_capacity;
        while (new_cap < (size_t)n)
            new_cap = new_cap * 2 + 1;  /* grow at least by 1 to avoid overflow */
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (new_cap + 1) * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        /* initialize new slots to 0 */
        for (size_t i = cache_capacity + 1; i <= new_cap; ++i)
            tmp[i] = 0;
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t factor = (seq_t)((seq_t)(i - 1) * (seq_t)7 + (seq_t)6);
        seq_t g = gcd(prev, factor);
        seq_t cur = (prev >= g) ? (prev - g) : (g - prev);
        cache[i] = cur;
    }
    cache_size = n;
    return cache[n];
}

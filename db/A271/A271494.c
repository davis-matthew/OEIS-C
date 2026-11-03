/*
 * A271494  Expansion of (1+16*x)/((1+4*x)*(1-8*x)).
 * https://oeis.org/A271494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 32*a(n-2); a(0)=1, a(1)=20
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 926 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)20;

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_max = 0;   /* number of entries allocated (size) */

    /* Initialise cache with the first two terms */
    if (cache == NULL) {
        cache = mem_new(seq_t, (size_t)2);
        if (!cache) return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)20;
        cache_max = 2;
    }

    /* If needed, resize the cache to hold up to n */
    idx_t start = cache_max;
    if (n + 1 > cache_max) {
        size_t newsize = (size_t)(n + 1);
        seq_t *tmp = mem_resize(seq_t, cache, newsize);
        if (!tmp) return (seq_t)0;            /* allocation failure */
        cache = tmp;
        cache_max = newsize;
    }

    /* Compute missing entries using the recurrence */
    for (idx_t i = start; i <= n; ++i) {
        /* i >= 2 here because start >= 2 for n >= 2 */
        cache[i] = (seq_t)4 * cache[i - 1] + (seq_t)32 * cache[i - 2];
    }

    return cache[n];
}

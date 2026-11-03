/*
 * A260304  a(n) = 5*a(n-1) - 5*a(n-2) for n>1, a(0)=2, a(1)=3.
 * https://oeis.org/A260304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 5*a(n-2) for n > 1; a(0) = 2, a(1) = 3
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 27 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* static cache for memoisation */
static seq_t *cache = NULL;        /* array holding computed values */
static idx_t cache_len = 0;        /* number of elements stored (>= 2 after init) */

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (cache_len == 0) {
        cache_len = 2;
        cache = mem_resize(seq_t, NULL, cache_len);
        if (!cache) return (seq_t)0;          /* fallback on allocation failure */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)3;
    }

    /* return already computed value */
    if (n < cache_len)
        return cache[n];

    /* expand cache to accommodate up to index n */
    cache = mem_resize(seq_t, cache, (n + 1));
    if (!cache) return (seq_t)0;  /* allocation failure */

    /* compute new terms using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = (seq_t)5 * cache[i-1] - (seq_t)5 * cache[i-2];
    }

    cache_len = n + 1;
    return cache[n];
}

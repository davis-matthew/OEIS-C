/*
 * A259593  Numerators of the other-side convergents to sqrt(3).
 * https://oeis.org/A259593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(0)=2, a(1)=3, a(2)=7, a(3)=12; a(n) = 4*a(n-2) - a(n-4) for n
 *            > 3 (OEIS A259593)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 32, u64 n <= 66, u128 n <= 133,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* memoisation array      */
static size_t cache_cap = 0;         /* number of elements held */

static const seq_t init[4] = {2, 3, 7, 12};

/* Expand the cache so that it contains indices 0..n inclusive */
static void ensure_cache(size_t n)
{
    if (n < cache_cap) return;          /* already available */

    size_t new_cap = n + 1;              /* one more for inclusive n */
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!cache) abort();

    /* initialise the first four terms if the array is new or incomplete */
    if (cache_cap < 4) {
        for (size_t i = 0; i < 4; ++i) cache[i] = init[i];
    }

    /* compute any missing terms */
    for (size_t i = cache_cap; i <= n; ++i) {
        if (i >= 4)                      /* recurrence applies only for i >= 4 */
            cache[i] = 4 * cache[i-2] - cache[i-4];
    }

    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* handle the first four terms directly */
    if ((idx_t)0 == n) return 2;
    if ((idx_t)1 == n) return 3;
    if ((idx_t)2 == n) return 7;
    if ((idx_t)3 == n) return 12;

    /* for n >= 4, use the memoised recurrence */
    ensure_cache((size_t)n);
    return cache[(size_t)n];
}

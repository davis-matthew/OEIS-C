/*
 * A251895  Numbers n such that the sum of the octagonal numbers N(n) and
 *          N(n+1) is equal to another octagonal number.
 * https://oeis.org/A251895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+1154*a(n-2)-1154*a(n-3)-a(n-4)+a(n-5) Initial
 *            values: a(1) = 0 a(2) = 30 a(3) = 368 a(4) = 34814 a(5) = 424864
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache[0] unused, cache[1] = a(1) */
static size_t cache_cap = 0;     /* number of allocated slots (incl. 0) */

/* Ensure cache has at least 'need' slots (0..need-1) */
static void ensure_cache(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = (cache_cap < 8) ? 8 : cache_cap * 2;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) { /* allocation failure: abort */
        abort();
    }
    /* Zero extend new entries to avoid garbage in subtraction */
    for (size_t i = cache_cap; i < new_cap; ++i)
        new_cache[i] = 0;
    cache = new_cache;
    cache_cap = new_cap;
}

/* Initialize the first five terms if not already done */
static void init_cache(void)
{
    if (cache_cap < 6) {       /* need indices 0..5 */
        ensure_cache(6);
    }
    if (cache[1] == 0) {       /* lazily initialise only once */
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)30;
        cache[3] = (seq_t)368;
        cache[4] = (seq_t)34814;
        cache[5] = (seq_t)424864;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* behaviour below offset is unspecified */

    /* Convert to size_t for indexing; idx_t is unsigned and fits */
    size_t idx = (size_t)n;
    ensure_cache(idx + 1);      /* cache[0..idx] */

    init_cache();

    /* Already computed? */
    if (idx < cache_cap && cache[idx] != 0)
        return cache[idx];

    /* Compute up to required index */
    for (size_t i = 6; i <= idx; ++i) {
        seq_t s1 = cache[i-1] + cache[i-5];
        s1 += (seq_t)1154 * cache[i-2];
        seq_t s2 = cache[i-4];
        s2 += (seq_t)1154 * cache[i-3];
        cache[i] = s1 - s2;
    }
    return cache[idx];
}

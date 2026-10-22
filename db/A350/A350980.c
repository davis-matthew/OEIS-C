/*
 * A350980  a(0) = 17, a(1) = 4700, thereafter a(n) = 254*a(n-1) - a(n-2) +
 *          378.
 * https://oeis.org/A350980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 17, * a(1) = 4700, * a(n) = 254*a(n-1) - a(n-2) + 378 (n
 *            >= 2)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache   = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

static void
ensure_cache_capacity(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 2;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t
a(idx_t n)
{
    /* initialise cache with the first two terms */
    if (cache_size < 2) {
        ensure_cache_capacity(2);
        cache[0] = (seq_t)17;
        cache[1] = (seq_t)4700;
        cache_size = 2;
    }

    size_t sn = (size_t)n;

    if (sn < cache_size)
        return cache[sn];

    /* ensure capacity up to index n */
    ensure_cache_capacity(sn + 1);

    for (size_t i = cache_size; i <= sn; ++i) {
        seq_t val = (seq_t)254 * cache[i-1] - cache[i-2] + (seq_t)378;
        cache[i] = val;
    }
    cache_size = sn + 1;
    return cache[sn];
}

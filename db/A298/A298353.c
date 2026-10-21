/*
 * A298353  a(n) = a(n-1) + a(n-2) + a([(n-1)/2]), where a(0) = 1, a(1) = 2,
 *          a(2) = 3.
 * https://oeis.org/A298353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(floor((n-1)/2)), a(0) = 1, a(1) = 2,
 *            a(2) = 3
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* memoised sequence values */
static idx_t cache_size = 0;    /* number of elements allocated in cache */

static void
ensure_cache(idx_t n)
{
    /* if the cache already contains a(n), nothing to do */
    if (n < cache_size)
        return;

    /* determine a new size that will hold at least indices 0..n */
    idx_t new_size = (cache_size == 0) ? 3 : cache_size;
    while (new_size <= n)
        new_size <<= 1;   /* double until it exceeds n */

    /* reallocate the cache to the new size */
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);      /* allocation failure */

    cache = tmp;

    /* initialise base values if this is the first allocation */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        if (new_size > 1)
            cache[1] = (seq_t)2;
        if (new_size > 2)
            cache[2] = (seq_t)3;
    }

    /* compute any new entries, starting from the old size */
    for (idx_t i = cache_size; i < new_size; ++i) {
        if (i <= 2)  /* base cases already set */
            continue;
        cache[i] = cache[i - 1] + cache[i - 2] + cache[(i - 1) / 2];
    }

    cache_size = new_size;
}

seq_t a(idx_t n)
{
    /* handle the first three terms directly */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)2;
    if (n == 2)
        return (seq_t)3;

    /* ensure that the cache contains a(n) */
    ensure_cache(n);
    return cache[n];
}

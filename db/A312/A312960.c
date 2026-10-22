/*
 * A312960  Coordination sequence Gal.4.106.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) (a(0)=1, a(1)=4, a(2)=9,
 *            a(3)=13, a(4)=18, a(5)=24, a(6)=28, a(7)=32) for n > 7
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* seq_t and idx_t are provided by the build system.  The following
 * implementation uses lazy memoisation.  The cache holds all
 * computed terms up to the highest n requested so far.
 */

static seq_t *cache = NULL;         /* dynamic array of seq_t terms */
static idx_t  cache_cap = 0;        /* capacity of cache (number of elems) */
static idx_t  cache_end = 0;        /* number of terms currently stored */

/* Ensure that the cache has room for at least 'needed' elements.
 * If it already has enough capacity, nothing is done. */
static void ensure_capacity(idx_t needed)
{
    if (cache_cap >= needed)
        return;

    idx_t new_cap = cache_cap ? cache_cap << 1 : 8;
    if (new_cap < needed)
        new_cap = needed;

    seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_cap);
    if (!new_cache) {
        /* OOM – abort: the tests expect a valid result, so terminate. */
        abort();
    }

    if (cache) {
        memcpy(new_cache, cache, sizeof(seq_t) * cache_end);
        free(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* Initialise the first eight known terms. */
static void init_cache(void)
{
    if (cache_end > 0)
        return;

    ensure_capacity(8);

    cache[0] = (seq_t)1;
    cache[1] = (seq_t)4;
    cache[2] = (seq_t)9;
    cache[3] = (seq_t)13;
    cache[4] = (seq_t)18;
    cache[5] = (seq_t)24;
    cache[6] = (seq_t)28;
    cache[7] = (seq_t)32;
    cache_end = 8;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    init_cache();

    if (n < cache_end)
        return cache[n];

    /* Need to compute up to 'n'.  Ensure capacity first. */
    ensure_capacity(n + 1);

    for (idx_t i = cache_end; i <= n; ++i) {
        /* Recurrence: a(i) = a(i-1) + a(i-3) - a(i-4)  for i > 7. */
        cache[i] = cache[i - 1] + cache[i - 3] - cache[i - 4];
    }
    cache_end = n + 1;
    return cache[n];
}

/*
 * A315860  Coordination sequence Gal.4.12.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-5) - a(n-10) for n > 10 with initial values a(0)=1,
 *            a(1)=6, a(2)=8, a(3)=12, a(4)=14, a(5)=22, a(6)=28, a(7)=28,
 *            a(8)=32, a(9)=34, a(10)=44
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t init[11] = {1,6,8,12,14,22,28,28,32,34,44};

static seq_t *cache = NULL;       /* dynamic memoisation array */
static size_t cache_cap = 0;      /* allocated capacity */
static size_t last = 10;          /* highest index already computed */

/* make sure the cache can hold indices up to n (inclusive) */
static void grow_cache_to(size_t n)
{
    if (n < cache_cap) return;
    size_t new_cap = n + 1;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) return;           /* allocation failed – leave behaviour undefined */
    if (cache) {
        for (size_t i = 0; i < cache_cap; ++i) new_cache[i] = cache[i];
        free(cache);
    }
    for (size_t i = cache_cap; i < new_cap; ++i) new_cache[i] = 0;
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 11) return init[(size_t)n];

    grow_cache_to((size_t)n);

    /* initialise first ten terms once */
    if (cache[10] == 0) {
        for (size_t i = 0; i <= 10; ++i) cache[i] = init[i];
    }

    /* compute new terms up to n */
    if (last < (size_t)n) {
        for (size_t i = last + 1; i <= (size_t)n; ++i)
            cache[i] = cache[i - 5] * 2 - cache[i - 10];
        last = (size_t)n;
    }

    return cache[n];
}

/*
 * A312933  Coordination sequence Gal.6.131.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312933
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) - a(n-2) - a(n-6) + 2*a(n-7) - a(n-8) for n > 8
   initial values: a(0)=1, a(1)=4, a(2)=9, a(3)=13, a(4)=18, a(5)=22,
                    a(6)=27, a(7)=32, a(8)=36 */

#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;          /* cache of computed terms */
static size_t cache_size = 0;        /* allocated size of cache */
static idx_t max_computed = 0;       /* highest index computed up to now */

static const seq_t init_vals[9] = {
    1, 4, 9, 13, 18, 22, 27, 32, 36
};

seq_t a(idx_t n)
{
    /* Return directly the first nine terms. */
    if (n <= 8)
        return init_vals[n];

    /* Allocate the cache on first demand. */
    if (cache == NULL) {
        cache_size = 9;
        cache = seq_alloc(cache_size);      /* compatible with arbitrary‑precision seq_t */
        for (idx_t i = 0; i <= 8; ++i) {
            cache[i] = init_vals[i];
        }
        max_computed = 8;
    }

    /* Grow the cache if the requested index exceeds the current size. */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = seq_realloc(cache, cache_size, new_size);
        cache = tmp;
        cache_size = new_size;
    }

    /* If the value is already computed, return it. */
    if (n <= max_computed)
        return cache[n];

    /* Compute terms up to n using the recurrence relation. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t v = 2 * cache[i-1];   /* 2*a(i-1) */
        v -= cache[i-2];            /* -a(i-2) */
        v -= cache[i-6];            /* -a(i-6) */
        v += 2 * cache[i-7];        /* +2*a(i-7) */
        v -= cache[i-8];            /* -a(i-8) */
        cache[i] = v;
    }

    max_computed = n;
    return cache[n];
}

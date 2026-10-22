/*
 * A342178  Product of first n central Delannoy numbers.
 * https://oeis.org/A342178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Prod_{k=1..n} A001850(k) (product of first n central
 *            Delannoy numbers)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   central_delannoy
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial value: a(0) = 1 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/central_delannoy.h"

/* Cache for a(n) values.  Implements an expanding array. */
static seq_t *cache      = NULL; /* cached a(0) … a(cache_used-1) */
static size_t cache_cap  = 0;    /* allocated capacity */
static size_t cache_used = 0;    /* number of computed entries */

seq_t a(idx_t n)
{
    size_t N = (size_t)n;

    /* Initialise cache on first call.  Allocate at least one slot. */
    if (cache_cap == 0)
    {
        cache_cap = 1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);          /* out of memory – abort */
        cache[0] = 1;                 /* a(0) = 1 */
        cache_used = 1;
    }

    /* Grow the cache if needed. */
    if (N >= cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap <= N) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);            /* out of memory – abort */
        cache = tmp;
        /* Zero any new slots (they will be overwritten). */
        for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = 0;
        cache_cap = new_cap;
    }

    /* Compute new values up to N (inclusive). */
    for (size_t i = cache_used; i <= N; ++i)
    {
        seq_t del = central_delannoy((idx_t)i);
        if (i == 0)
            cache[i] = 1;
        else
            cache[i] = cache[i - 1] * del;
    }
    cache_used = N + 1;

    return cache[N];
}

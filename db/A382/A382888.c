/*
 * A382888  The squarefree kernel of the n-th cubefree number.
 * https://oeis.org/A382888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = squarefree kernel of the n‑th cubefree number. The code
 *            generates cubefree numbers in increasing order, caches their
 *            squarefree kernel, and returns the cached value for any n >= 1
 * Range:     u8 n <= 214, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_realloc, test_cubefree_and_rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_realloc.h"   /* for seq_realloc() */
#include "../utils/test_cubefree_and_rad.h"

static seq_t *cache = NULL;        /* cached a(1) .. a(cache_len-1) */
static size_t cache_cap = 0;       /* allocated capacity of cache */
static size_t cache_len = 0;       /* number of elements actually stored */
static uint64_t next_candidate = 1;/* next integer to test for cubefree */

/*  Ensure that cache can hold at least 'needed' elements. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    cache = seq_realloc(cache, cache_cap, new_cap);
    cache_cap = new_cap;
}

/*  a(n) as defined above.  Computes values lazily and caches them. */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n < 1 */
        return (seq_t)0;

    ensure_cache_capacity(n);

    while (cache_len < n) {
        seq_t rad;
        if (test_cubefree_and_rad(next_candidate, &rad)) {
            cache[cache_len++] = rad;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}

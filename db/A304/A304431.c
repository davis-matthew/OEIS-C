/*
 * A304431  a(n+1) = 1 + min( 2*a(floor(n/2)), 3*a(floor(n/3)) ), with a(0) =
 *          0.
 * https://oeis.org/A304431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = 1 + min( 2*a(floor(n/2)), 3*a(floor(n/3)) ), a(0) = 0
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   alloc_seq_array, free_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* helpers supplied by the testing harness */
#include "../utils/alloc_seq_array.h"
#include "../utils/free_cache.h"

/* The sequence cache (values already computed) */
static seq_t *cache = NULL;
static size_t cache_capacity = 0;

/* extend the cache up to index n inclusive */
static void ensure_cache_upto(idx_t n)
{
    /* target is number of elements needed (index n -> size n+1) */
    size_t target = (size_t)n + 1;

    if (cache == NULL) {
        /* allocate for index 0 */
        cache = alloc_seq_array(1);
        cache[0] = (seq_t)0;
        cache_capacity = 1;
    }

    if (target <= cache_capacity)
        return;                      /* already have enough */

    /* remember the old size before reallocating */
    size_t old_capacity = cache_capacity;

    /* allocate new larger buffer */
    seq_t *new_cache = alloc_seq_array(target);
    /* copy old values */
    for (size_t i = 0; i < old_capacity; ++i)
        new_cache[i] = cache[i];
    /* free old buffer */
    free_cache(cache);
    cache = new_cache;
    cache_capacity = target;

    /* compute new values from old_capacity to target-1 */
    for (size_t i = old_capacity; i < target; ++i) {
        /* i is the target index, corresponding to n = i-1 in the recurrence */
        idx_t n = (idx_t)(i - 1);
        idx_t half  = n / 2;
        idx_t third = n / 3;
        seq_t v2 = 2 * cache[half];
        seq_t v3 = 3 * cache[third];
        cache[i] = 1 + (v2 < v3 ? v2 : v3);
    }
}

/* public function requested by the problem */
seq_t a(idx_t n)
{
    ensure_cache_upto(n);
    return cache[(size_t)n];
}

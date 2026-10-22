/*
 * A353698  Number of integer partitions of n whose product equals their
 *          length.
 * https://oeis.org/A353698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   enum_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Number of integer partitions of n whose product equals their length */
/* Implementation builds up a cache of values as n increases. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/enum_factor.h"

/* Static cache (index → value).  All entries are initialized to 0. */
static seq_t *cache = NULL;
static idx_t cache_len = 0;           /* current size of cache */

/* Compute and cache all terms up to `limit`. */
static void
compute_up_to(idx_t limit)
{
    if (limit < cache_len)
        return;                    /* already computed */

    /* Grow the cache.  Cast the void* from realloc to seq_t*. */
    size_t new_len = (size_t)(limit + 1);
    seq_t *new_cache = (seq_t *)realloc(cache,
                                        new_len * sizeof(seq_t));
    if (!new_cache && new_len > 0) {
        abort();                   /* allocation failed */
    }

    /* Initialise new entries to zero. */
    for (size_t i = cache_len; i < new_len; ++i)
        new_cache[i] = 0;

    cache = new_cache;
    cache_len = new_len;

    /* Enumerate contributions from new k values. */
    for (idx_t k = cache_len - new_len; k <= limit; ++k) {
        if (k == 0)
            continue;               /* no partition has product 0 */
        if (k == 1) {               /* partition [1] of 1 */
            cache[1] += 1;
            continue;
        }
        enum_factor(k, k, 2, 0, 0, cache, limit);
    }
}

/* Public function required by the testing harness. */
seq_t a(idx_t n)
{
    if (n + 1 > cache_len)
        compute_up_to(n);
    return cache[n];
}

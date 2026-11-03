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
 * Validated: standard, pass, 107 terms through n = 9839 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Number of integer partitions of n whose product equals their length */
/* Implementation builds up a cache of values as n increases. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Recursive enumeration of all factorizations of `rem` using factors ≥ `start`.
   orig_k  – the original value of k (the product = length).
   sum_non – sum of the factors chosen so far.
   parts   – number of factors chosen so far.
   limit   – maximum n to count (cache size - 1). */
static void
enum_factor(idx_t orig_k, idx_t rem, idx_t start,
            idx_t sum_non, size_t parts,
            seq_t *cache, idx_t limit)
{
    if (rem == 1) {
        idx_t n = sum_non + (orig_k - parts);
        if (n <= limit)
            cache[n] += 1;
        return;
    }

    for (idx_t f = start; f * f <= rem; ++f) {
        if (rem % f == 0) {
            enum_factor(orig_k, rem / f, f,
                        sum_non + f, parts + 1,
                        cache, limit);
        }
    }

    /* Remaining factor as a single part */
    if (rem >= start) {
        enum_factor(orig_k, 1, rem,
                    sum_non + rem, parts + 1,
                    cache, limit);
    }
}

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
    seq_t *new_cache = mem_resize(seq_t, cache, new_len);
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

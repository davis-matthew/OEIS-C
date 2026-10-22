/*
 * A353857  Numbers k such that the k-th composition in standard order has
 *          run-sum trajectory ending in a singleton.
 * https://oeis.org/A353857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   ends_in_singleton, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A353857: a(n) is the n‑th integer k such that the k‑th composition
   in standard (graded reverse‑lexicographic) order has a run‑sum
   trajectory that ends in a singleton.  This function computes the
   sequence value for any n ≥ 1.  The algorithm enumerates k = 1,2,…
   tests each k for the required property and caches all accepted k
   values so that subsequent calls are fast. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/ends_in_singleton.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* offset starts at 1 */

    /* Static cache of the first few terms that have already been found. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_cnt = 0;
    static idx_t next_k = 1;       /* next k to test */

    while (cache_cnt < n) {
        if (ends_in_singleton(next_k)) {
            /* Grow the cache if necessary. */
            if (cache_cnt >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 128;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_cnt++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return cache[n - 1];
}

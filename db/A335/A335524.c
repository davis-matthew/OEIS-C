/*
 * A335524  Numbers k such that the k-th composition in standard order
 *          (A066099) avoids the pattern (2,2,1).
 * https://oeis.org/A335524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   avoids_pattern, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A335524:
   a(n) = n-th integer k such that the k-th composition (in standard order)
   avoids the pattern (2,2,1). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/avoids_pattern.h"

seq_t a(idx_t n)
{
    if (n <= 0) return 0;          /* safety for indices < 1 */

    /* Static cache of found k-values. */
    static seq_t *cache     = NULL;
    static size_t cache_cap = 0;
    static idx_t  cache_len = 0;
    static idx_t  next_k    = 0;   /* k to test next */

    while (cache_len < n) {
        if (avoids_pattern(next_k)) {
            /* Allocate or grow the cache. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp;
                if (cache_cap == 0) {
                    tmp = seq_alloc(new_cap);
                    if (!tmp) abort();
                } else {
                    tmp = seq_realloc(cache, cache_cap, new_cap);
                    if (!tmp) abort();
                }
                cache     = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[n - 1];
}

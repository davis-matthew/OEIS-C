/*
 * A247016  Triangular numbers A000217 composed of only curved digits {0, 2,
 *          3, 5, 6, 8, 9}.
 * https://oeis.org/A247016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 71, u32 n <= 2448, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   add_to_cache, is_curved
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence of triangular numbers composed only of curved
   digits {0,2,3,5,6,8,9} by scanning triangular numbers incrementally. */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>   /* used only in C mode realloc copy */
#include "../utils/add_to_cache.h"
#include "../utils/is_curved.h"

/* External linkage function requested by the test harness. */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;   /* safety guard, not required */

    /* static state – persists across calls in any order */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t next_tri_n = 0;   /* triangular index (0,1,2,…) */

    size_t idx = (size_t)(n - (idx_t)1);   /* zero‑based cache index */

    while (cache_len <= idx) {
        /* compute the next triangular number safely in seq_t */
        seq_t tri = (seq_t)next_tri_n *
                    ((seq_t)next_tri_n + (seq_t)1) /
                    (seq_t)2;
        if (is_curved(tri))
            add_to_cache(&cache, &cache_cap, &cache_len, tri);
        ++next_tri_n;
    }
    return cache[idx];
}

/*
 * A382139  Number of matchings of [2n] with no coupled arcs.
 * https://oeis.org/A382139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (recurrence):
   a(0) = 1, a(1) = 1, a(2) = 1
   a(n) = (2*n-1)*a(n-1) - 2*(n-1)*a(n-2) + 4*(n-1)*(n-2)*a(n-3)   for n ≥ 3 */
#include <stddef.h>
#include <stdlib.h>
/* Use allocator that works for both built‑in and arbitrary‑precision types */
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms. */
    static seq_t *cache     = NULL;
    static idx_t current_max = (idx_t)-1;   /* no terms computed yet */
    static size_t cache_cap   = 0;          /* allocated capacity */

    /* ---------- first call: allocate and initialise ---------- */
    if (current_max == (idx_t)-1) {
        size_t need = (size_t)(n + 1);      /* at least one element */
        if (need == 0)
            need = 1;
        cache = alloc_seq_array(need);
        cache_cap = need;
        cache[0] = (seq_t)1;
        if (n >= 1)
            cache[1] = (seq_t)1;
        if (n >= 2)
            cache[2] = (seq_t)1;
        current_max = (n >= 2) ? (idx_t)2 : n;

        /* Compute missing terms up to n */
        for (idx_t i = current_max + 1; i <= n; ++i) {
            seq_t term1 = ((seq_t)2 * (seq_t)i - (seq_t)1) * cache[i - 1];
            seq_t term2 = ((seq_t)2 * (seq_t)(i - 1)) * cache[i - 2];
            seq_t term3 = ((seq_t)4 * (seq_t)(i - 1) * (seq_t)(i - 2))
                          * cache[i - 3];
            cache[i] = term1 - term2 + term3;
        }
        return cache[n];
    }

    /* ---------- terms already cached ---------- */
    if (n <= current_max)
        return cache[n];

    /* ---------- need to grow the cache ---------- */
    size_t new_cap = (size_t)(n + 1);
    if (new_cap > cache_cap) {
        seq_t *new_cache = alloc_seq_array(new_cap);
        /* copy existing terms */
        for (idx_t i = 0; i <= current_max; ++i)
            new_cache[i] = cache[i];
        free_seq_array(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms */
    for (idx_t i = current_max + 1; i <= n; ++i) {
        seq_t term1 = ((seq_t)2 * (seq_t)i - (seq_t)1) * cache[i - 1];
        seq_t term2 = ((seq_t)2 * (seq_t)(i - 1)) * cache[i - 2];
        seq_t term3 = ((seq_t)4 * (seq_t)(i - 1) * (seq_t)(i - 2))
                      * cache[i - 3];
        cache[i] = term1 - term2 + term3;
    }
    current_max = n;
    return cache[n];
}

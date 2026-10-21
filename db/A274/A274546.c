/*
 * A274546  Numbers m such that 5*m is squarefree.
 * https://oeis.org/A274546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by testing successive integers for squarefreeness
   and non‑divisibility by 5.  The results are cached so the
   function can be called in any order. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */
    static idx_t next_candidate = 1;   /* next integer to test */

    /* quick return if requested term is already known */
    if (cache_len >= n)
        return cache[n - 1];

    /* generate terms until the cache reaches the requested index */
    while (cache_len < n) {
        /* skip numbers divisible by 5 */
        if ((next_candidate % 5U) != 0 && is_squarefree((seq_t)next_candidate)) {
            /* grow the cache if needed */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();       /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}

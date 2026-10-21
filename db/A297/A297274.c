/*
 * A297274  Numbers whose base-11 digits have equal down-variation and
 *          up-variation; see Comments.
 * https://oeis.org/A297274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 5967, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_property
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A297274: numbers whose base-11 digits have equal down-variation and up-variation.
   Computed by brute force search and memoized. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_property.h"

seq_t a(idx_t n)
{
    /* static memoization data */
    static seq_t *cache = NULL;        /* array of found terms */
    static size_t cache_cap = 0;        /* capacity of the array */
    static idx_t cache_size = 0;        /* number of terms stored */
    static seq_t next_candidate = 1;   /* next integer to test */

    /* initialise cache */
    if (cache == NULL) {
        cache_cap = 256;                /* initial capacity */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();            /* unrecoverable if out of memory */
    }

    /* generate terms until we have at least n of them */
    while (cache_size < n) {
        seq_t c = next_candidate;
        while (!digit_property(c))
            ++c;                       /* find next valid term */

        /* grow cache if needed */
        if (cache_size == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[(size_t)cache_size] = c;
        ++cache_size;
        next_candidate = c + 1;
    }

    return cache[(size_t)(n - 1)];      /* sequence is 1‑based */
}

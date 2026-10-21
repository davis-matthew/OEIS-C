/*
 * A297134  Numbers having an up-first zigzag pattern in base 6; see Comments.
 * https://oeis.org/A297134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   zigzag_up_first
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A297134: Numbers having an up‑first zigzag pattern in base 6
   (found by brute‑force search). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/zigzag_up_first.h"

/* OEIS a(n) function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined; safe fallback */

    static seq_t *cache      = NULL; /* stored qualifying numbers */
    static idx_t cache_size  = 0;     /* number of entries in cache */
    static idx_t cache_cap   = 0;     /* allocated capacity */
    static seq_t last_checked = 1;    /* next candidate to test */

    if (n <= cache_size)            /* quick lookup if we already have it */
        return cache[n - 1];

    /* Expand cache until we have at least n values. */
    while (cache_size < n) {
        if (zigzag_up_first(last_checked)) {
            if (cache_size == cache_cap) {           /* grow the cache */
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache,
                                               new_cap * sizeof(seq_t));
                if (!tmp) return 0;                  /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = last_checked;
        }
        ++last_checked;   /* advance to next candidate */
    }
    return cache[n - 1];
}

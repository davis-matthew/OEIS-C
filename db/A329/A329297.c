/*
 * A329297  Numbers whose digits are in nondecreasing order in bases 7 and 8.
 * https://oeis.org/A329297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 39, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   nondecreasing_in_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose digits are in nondecreasing order in bases 7 and 8. (A329297) */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/nondecreasing_in_base.h"

/* Return the n‑th term of the sequence, with offset 1 (a(1)=0). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* undefined, but guard */

    /* Static state for incremental construction. */
    static seq_t *cache   = NULL; /* array of found terms */
    static size_t cap    = 0;     /* current capacity of cache   */
    static size_t size   = 0;     /* number of valid terms stored */
    static idx_t next    = 0;     /* next integer to test         */

    /* Grow the cache until we have at least n terms. */
    while (size < (size_t)n) {
        /* Ensure sufficient storage. */
        if (cache == NULL || size == cap) {
            size_t new_cap = cap ? cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {            /* allocation failure */
                if (cache) free(cache);
                return 0;
            }
            cache = tmp;
            cap   = new_cap;
        }

        /* Test the next integer. */
        uint64_t v = next++;           /* use 64‑bit to hold the test value */
        if (nondecreasing_in_base(v, 7) && nondecreasing_in_base(v, 8)) {
            cache[size++] = (seq_t) v; /* truncate to seq_t if necessary */
        }
    }

    return cache[n - 1];
}

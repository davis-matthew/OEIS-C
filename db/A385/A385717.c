/*
 * A385717  a(n) = a(n-1) + a(n-2) + a(n-3), with a(1) = 4, a(2) = 13, a(3) =
 *          42.
 * https://oeis.org/A385717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3), with a(1)=4, a(2)=13, a(3)=42
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   alloc_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq.h"

/* Computes term a(n) of OEIS sequence A385717.
 * The function caches results so that calls in any order are fast.
 * It returns 0 for n<1 (offset is 1). */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;     /* number of slots allocated (incl. index 0) */
    static idx_t max_computed = 0;   /* largest index for which a value is known */

    if (n < 1) return 0;         /* out of range; not part of the defined sequence */

    /* Determine how many slots we need (index 0 is unused) */
    idx_t need = n + 1;           /* need indices 1..n inclusive */

    /* Allocate a new cache if necessary */
    if (cache_size < need) {
        /* Allocate a fresh buffer and copy any previously cached entries */
        seq_t *new_cache = alloc_seq((size_t)need);
        if (!new_cache) return 0;          /* allocation failed */

        if (cache) {
            for (idx_t i = 1; i <= max_computed; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }

        cache = new_cache;
        cache_size = need;

        /* Initialise the basic terms (they may be needed for later recurrence) */
        if (need > 1) cache[1] = (seq_t)4;
        if (need > 2) cache[2] = (seq_t)13;
        if (need > 3) cache[3] = (seq_t)42;

        /* Update the largest computed index after initialisation */
        if (need - 1 <= 3)            /* n <= 3: we already have the needed value */
            max_computed = (idx_t)(need - 1);
        else
            max_computed = 3;          /* we have at least the first three terms */
    }

    /* If we haven't computed up to n yet, extend the cache via the recurrence */
    if (max_computed < n) {
        for (idx_t i = max_computed + 1; i <= n; ++i)
            cache[i] = cache[i-1] + cache[i-2] + cache[i-3];
        max_computed = n;
    }

    return cache[n];
}

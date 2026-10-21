/*
 * A290156  Tribonacci-like sequence of composite numbers with a(0) =
 *          99202581681909167232, a(1) = 67600144946390082339 and a(2) =
 *          139344212815127987596.
 * https://oeis.org/A290156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3) a(0)=99202581681909167232,
 *            a(1)=67600144946390082339, a(2)=139344212815127987596
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 71, big any n
 * Helpers:   seq_from_str, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoizes the sequence in a static cache.
   It supports any seq_t type defined by the build (8‑, 16‑, 32‑, 64‑,
   128‑bit unsigned integers or an arbitrary‑precision integer class). */

#include <stddef.h>
#include "../utils/seq_from_str.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n) {
    /* static cache: values already computed */
    static seq_t *cache = NULL;   /* array of seq_t */
    static size_t cache_cap = 0;  /* allocated capacity */
    static size_t cache_size = 0; /* number of values stored */

    /* Ensure we have initial values */
    if (cache_size == 0) {
        size_t init_cap = 10;
        cache = seq_realloc(NULL, 0, init_cap);
        cache_cap = init_cap;
        cache[0] = seq_from_str("99202581681909167232");
        cache[1] = seq_from_str("67600144946390082339");
        cache[2] = seq_from_str("139344212815127987596");
        cache_size = 3;
    }

    /* If the requested index is already cached, return it */
    if (n < cache_size) {
        return cache[n];
    }

    /* Grow the cache to accommodate up to n */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 10;
        while (new_cap <= n + 1) {
            new_cap *= 2;
        }
        cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }

    /* Compute missing terms iteratively */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        /* a(i) = a(i-1)+a(i-2)+a(i-3) */
        cache[i] = cache[i-1] + cache[i-2] + cache[i-3];
    }
    cache_size = n + 1;

    return cache[n];
}

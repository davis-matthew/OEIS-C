/*
 * A369347  Numbers whose decimal expansion is quasiperiodic.
 * https://oeis.org/A369347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th positive integer whose decimal representation
 *            (without leading zeros) can be covered by overlapping
 *            occurrences of one of its proper prefixes. The algorithm tests
 *            numbers incrementally, stopping when the required index is
 *            found. It works for any unsigned seq_t larger than 0 and re...
 * Range:     u8 n <= 11, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   has_quasiperiodic_cover
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/has_quasiperiodic_cover.h"

/* Main sequence generator with memoisation. */
seq_t a(idx_t n)
{
    /* Persistent state across calls. */
    static seq_t *cache = NULL;   /* array of found terms          */
    static size_t cacheSize = 0;  /* number of terms stored        */
    static size_t cacheCap  = 0;  /* allocated capacity            */
    static seq_t candidate = 0;   /* next value to examine        */
    static bool initialized = false; /* first–call initialisation */
    static idx_t  count    = 0;     /* terms cached so far (count>=0) */

    /* Initialise the first candidate once. */
    if (!initialized) {
        candidate = (seq_t)11;
        initialized = true;
    }

    /* If we have already computed enough terms, return the cached one. */
    if (cacheSize >= n && n > 0)
        return cache[n - 1];

    /* Generate terms until we reach the requested index. */
    while (count < n) {
        if (has_quasiperiodic_cover(candidate)) {
            /* Grow cache if needed. */
            if (cacheSize == cacheCap) {
                size_t newCap = cacheCap == 0 ? 16 : cacheCap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, newCap * sizeof(seq_t));
                if (!tmp)                     /* allocation failed */
                    return (seq_t)0;
                cache = tmp;
                cacheCap = newCap;
            }
            cache[cacheSize++] = candidate;
            ++count;

            /* Decide whether to return now; store return value first. */
            seq_t ret = candidate;
            ++candidate;                /* prepare next search value */
            if (count == n)
                return ret;
            continue;                   /* continue searching */
        }
        ++candidate;   /* next integer to test */
    }

    /* The loop above guarantees we have returned the value. */
    return (seq_t)0;
}

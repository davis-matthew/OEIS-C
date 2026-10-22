/*
 * A321493  Numbers m such that m and m+1 both have at least 3, but m or m+1
 *          has at least 4 distinct prime factors.
 * https://oeis.org/A321493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A321493
   a(n) = the n-th integer m (m ≥ 1) such that
   -- m and m+1 each have at least three distinct prime factors, and
   -- at least one of {m, m+1} has at least four distinct prime factors.
   The function searches for such integers incrementally and caches the
   results, so that independent calls with arbitrary n are handled
   efficiently. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    /* Static table of found values, grows as required. */
    static seq_t *cache = NULL;     /* pointer to the array of values   */
    static size_t sz   = 0;         /* number of values stored          */
    static size_t cap  = 0;         /* allocated capacity of cache      */
    static uint64_t cand = 1;       /* next integer to test             */

    /* Initialise the cache on first call */
    if (cache == NULL) {
        cap  = 8;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* out–of‑memory */
    }

    /* Generate terms until we have at least n of them. */
    while (sz < (size_t)n) {
        uint64_t m   = cand;
        int      om  = count_distinct_primes(m);
        int      om1 = count_distinct_primes(m + 1);

        if (om >= 3 && om1 >= 3 && (om >= 4 || om1 >= 4)) {
            /* Ensure enough space. NOTE: realloc() is safe in C++ when
               seq_t is a trivial type; the function handles the case
               where seq_t is a class simply because we never rely on
               its constructor (the array is only written to via
               assignment). */
            if (sz == cap) {
                cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                if (!tmp) {            /* out–of‑memory */
                    free(cache);
                    cache = NULL;
                    sz = 0;
                    return (seq_t)0;
                }
                cache = tmp;
            }
            cache[sz++] = (seq_t)m;  /* store the found value */
        }
        cand++;                       /* advance to next candidate */
    }

    return cache[n - 1];
}

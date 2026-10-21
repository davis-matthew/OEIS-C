/*
 * A270642  The sequence a of 1's and 2's starting with (1,1,2,2) such that
 *          a(n) is the length of the (n+2)nd run of a.
 * https://oeis.org/A270642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A270642 - construction by runs
   The sequence a(1)=1, a(2)=1, a(3)=2, a(4)=2, …
   For each positive integer n, a(n) is the value at position n
   in the infinite word built by alternating runs of 1's and 2's.
   Let run r (r ≥ 1) be the r‑th block of equal symbols.
   The first two runs have length 2.
   For r ≥ 3 the length of run r is a(r − 2) (using the value
   we are constructing).
   Run r contains 1 if r is odd, 2 if r is even.
   This function generates terms sequentially, caching already
   computed values, and works for any n ≥ 1 in any order of calls. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    /* offset 1: cache[0] holds a(1) */
    static seq_t *cache = NULL;   /* dynamic array with the sequence  */
    static size_t cap   = 0;      /* allocated capacity                */
    static size_t len   = 0;      /* number of terms stored            */
    static idx_t  run   = 0;      /* number of runs that have been added */

    if (n == 0u) return (seq_t)0; /* defined only for n ≥ 1 */

    /* Ensure we have at least n terms in cache */
    while (len < (size_t)n) {
        ++run;  /* start a new run */

        /* length of this run */
        idx_t run_len;
        if (run <= 2) {
            run_len = 2;  /* first two runs have length 2 */
        } else {
            /* run >= 3 : length = a(run‑2) */
            run_len = (idx_t)cache[(size_t)(run - 3)]; /* cache[0] is a(1) */
        }

        /* value of the run: 1 for odd runs, 2 for even runs */
        seq_t run_val = (run & 1u) ? (seq_t)1 : (seq_t)2;

        /* Grow the cache if space is insufficient */
        if (cap < len + run_len) {
            size_t new_cap = cap ? cap << 1 : 8;
            while (new_cap < len + run_len) new_cap <<= 1;

            seq_t *tmp = seq_alloc_new_array(new_cap);
            if (!tmp) abort(); /* allocation failure */

            /* copy existing terms */
            for (size_t i = 0; i < len; ++i) tmp[i] = cache[i];

            /* free old array */
            seq_free_array(cache);
            cache = tmp;
            cap   = new_cap;
        }

        /* append run_val run_len times */
        for (idx_t i = 0; i < run_len; ++i) {
            cache[len++] = run_val;
        }
    }

    return cache[(size_t)(n - 1u)];
}

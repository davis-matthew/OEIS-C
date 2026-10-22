/*
 * A353852  Numbers k such that the k-th composition in standard order (row k
 *          of A066099) has all distinct run-sums.
 * https://oeis.org/A353852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   distinct_run_sums
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n‑th nonnegative integer k whose k‑th composition in standard order
 * (graded reverse‑lexicographic, A066099) has all distinct run‑sums.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/distinct_run_sums.h"

/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/*  The main sequence function.  It memoises previously found values in a
 *  dynamic array so that calls in arbitrary order remain fast.          */
seq_t a(idx_t n)
{
    /* Cache of valid k values.  The array holds idx_t values, cast to seq_t
     * on return.  */
    static idx_t *cache = NULL;
    static size_t cache_sz = 0, cache_cap = 0;
    static idx_t next_k = 0;   /* next candidate k to test */

    /* Initialise cache on first call. */
    if (!cache) {
        cache_cap = 1024;
        cache = (idx_t *)malloc(cache_cap * sizeof(idx_t));
        cache_sz = 0;
    }

    /* Generate terms until we have at least n+1 entries. */
    while (cache_sz <= (size_t)n) {
        if (distinct_run_sums(next_k)) {
            if (cache_sz == cache_cap) {
                cache_cap *= 2;
                cache = (idx_t *)realloc(cache, cache_cap * sizeof(idx_t));
            }
            cache[cache_sz++] = next_k;
        }
        ++next_k;
    }

    return (seq_t)cache[n];
}

/*
 * A287831  Number of sequences over the alphabet {0,1,...,9} such that no two
 *          consecutive terms have distance 8.
 * https://oeis.org/A287831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A287831: Number of sequences over {0,…,9} such that no two
   consecutive terms have distance 8.
   Recurrence:
       a(n) = 9*a(n-1) + 6*a(n-2),  a(0)=1, a(1)=10
   The function memoises previously computed values so that calls
   may occur in any order.  All arithmetic is performed in seq_t, so
   the function works for 8‑, 16‑, 32‑, 64‑ or 128‑bit unsigned types
   and for arbitrary‑precision integer types. */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache of already computed values */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;   /* capacity of the cache array       */
    static size_t  len   = 0;   /* number of values stored (first
                                   n+1 entries of the sequence) */

    /* Ensure that the cache can store indices up to n.  We reserve
       at least two elements so that the recurrence for a(1) can be
       written uniformly. */
    if ((size_t)n + 1 > cap) {
        size_t needed = (size_t)n + 1;
        size_t new_cap = cap ? cap << 1 : 2;   /* start with 2 slots */
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE); /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise the first two terms if this is the first call. */
    if (len == 0) {
        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)10;  /* a(1) */
        len = 2;
    }

    /* If the requested value is already in the cache, return it. */
    if ((size_t)n < len)
        return cache[(size_t)n];

    /* Compute missing terms up to n using the recurrence. */
    for (size_t i = len; i <= (size_t)n; ++i) {
        cache[i] = (seq_t)9 * cache[i-1] + (seq_t)6 * cache[i-2];
    }
    len = (size_t)n + 1;

    return cache[(size_t)n];
}

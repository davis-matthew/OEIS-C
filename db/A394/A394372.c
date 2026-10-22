/*
 * A394372  a(1)=1, a(2)=2, a(3)=3; for n > 3, a(n) = a(n-1) + gcd(n, a(n-2) *
 *          a(n-3)).
 * https://oeis.org/A394372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + gcd(n, a(n-2) * a(n-3)), a(1) = 1, a(2) = 2,
 *            a(3) = 3
 * Range:     u8 n <= 11, u16 n <= 47, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoises computed terms so that any order of
   calls to a(idx_t n) is supported, and it works with the
   arbitrary‑precision or normal integer types supported by the
   build. */

#include <stdlib.h>        /* malloc, free */
#include <stddef.h>        /* size_t */
#include "../utils/gcd.h"  /* seq_t gcd(seq_t, seq_t) */

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Static cache: indices [1 .. max_index] are valid */
    static seq_t *cache      = NULL;  /* allocated array of seq_t */
    static size_t cap        = 0;     /* current allocation size */
    static idx_t  max_index  = 0;     /* highest n that has been computed */

    /* Request enough space for indices 1..n (inclusive) and a guard at 0.
       Always keep at least 4 slots so that the initial terms 1..3 can be
       stored without reallocating before they are set. */
    if (cap <= (size_t)n) {
        size_t needed = (size_t)n + 1;           /* indices 0 .. n */
        if (needed < 4) needed = 4;              /* minimum needed slots */
        seq_t *new_cache = (seq_t *)malloc(needed * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed; the sequence cannot be computed. */
            return 0;
        }
        /* Copy previously computed values into the new array */
        if (cache) {
            for (size_t i = 1; i < cap; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }
        cache = new_cache;
        cap   = needed;
    }

    /* Initialise the first three terms if not already done. */
    if (max_index < 3) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)3;
        max_index = 3;
    }

    /* Compute missing terms up to n.  The loop starts at the next
       uncomputed index and stops once the requested term is available. */
    for (idx_t i = max_index + 1; i <= n; ++i) {
        /* Compute a(n) = a(n-1) + gcd(n, a(n-2) * a(n-3)) */
        seq_t prod = cache[i-2] * cache[i-3];
        seq_t g    = gcd((seq_t)i, prod);
        cache[i]   = cache[i-1] + g;
    }
    if (n > max_index) max_index = n;

    return cache[n];
}

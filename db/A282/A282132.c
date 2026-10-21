/*
 * A282132  Imaginary part of n!*Sum_{k=1..n} i^(k-1)/k, where i is sqrt(-1).
 * https://oeis.org/A282132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A282132: Imaginary part of n!*Sum_{k=1..n} i^{k-1}/k
 * Recurrence: a(1) = 0
 * a(n+1) = a(n)*(n+1) + n! * sin(Pi*n/2),  n ≥ 1
 * sin(Pi*n/2) is 0 for even n, +1 for n ≡ 1 (mod 4), -1 for n ≡ 3 (mod 4)
 *
 * The implementation memoises computed values so that calls in arbitrary
 * order run in O(max(n)) time and constant space per call (aside from the
 * cache).  All arithmetic uses only the integer operators allowed by the
 * specification and is valid for the arbitrary‑precision build as well as
 * for fixed‑size integer builds.  It never hard‑codes terms from the OEIS
 * b‑file. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>   /* size_t */

#include "../utils/seq_alloc_new_array.h"   /* provides seq_alloc_new_array */

/* External linkage as required */
seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;  /* cache[1..max] holds a(1)..a(max) */
    static size_t cached_n   = 0;     /* highest index cached */
    static seq_t fact_current = 0;    /* (cached_n)!  */
    static bool       init    = false;

    if (!init) {
        /* initialise for n>=1 */
        init      = true;
        cached_n  = 1;
        cache     = seq_alloc_new_array(2);  /* indices 0 and 1 */
        cache[1]  = 0;      /* a(1) */
        fact_current = (seq_t)1;   /* 1! = 1   */
    }

    if (n <= 1) {          /* n = 1 (or 0) */
        return (n == 1) ? cache[1] : (seq_t)0;
    }

    if ((size_t)n > cached_n) {
        /* grow cache to hold a(1)..a(n) */
        seq_t *new_cache = seq_alloc_new_array((size_t)n + 1);
        for (size_t i = 0; i <= cached_n; ++i)
            new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;

        /* compute missing terms */
        for (idx_t m = cached_n + 1; m <= n; ++m) {
            idx_t prev = m - 1;                 /* index for a(prev) */
            int   s    = 0;                     /* sin(π*prev/2) value */

            switch (prev & 3) {                 /* prev mod 4 */
                case 1: s = 1; break;
                case 3: s = -1; break;
            }

            seq_t term = cache[prev];
            term = term * (seq_t)m;              /* a(prev)*m */
            if (s) {
                if (s > 0) term = term + fact_current;
                else       term = term - fact_current;
            }
            cache[m] = term;

            fact_current = fact_current * (seq_t)m;  /* become m! for next step */
        }
        cached_n = (size_t)n;
    }

    return cache[(size_t)n];
}

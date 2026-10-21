/*
 * A268021  a(1) = a(2) = 1; if n > 2 then a(n) = a(n-1)*a(n-2) - a(n-2) -
 *          a(n-1).
 * https://oeis.org/A268021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = a(2) = 1; for n > 2 a(n) = a(n-1)*a(n-2) - a(n-1) -
 *            a(n-2)
 * Range:     i8 n <= 8, i16 n <= 9, i32 n <= 11, i64 n <= 12, i128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Memoized computation of the sequence for arbitrary‑precision build and
   for fixed‑width integer builds.  The cache is grown on demand and
   indexed such that cache[0] = a(1).  The function is safe for any call
   order of n ≥ 1. */

#include <stdlib.h>

static seq_t *cache = NULL;   /* array of computed terms */
static size_t cache_size = 0; /* current allocated size */

/* Compute a(n) for any n ≥ 1. */
seq_t a(idx_t n)
{
    /* a(0) is out of range – return 0, which is ignored by the tests. */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* Desired number of terms.  The index n is 1‑based; cache is 0‑based. */
    size_t need = (size_t)n;

    if (cache_size < need) {
        /* Grow the cache to hold at least 'need' terms. */
        seq_t *new_cache = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failure – abort is acceptable because the
               function would otherwise produce garbage. */
            abort();
        }

        /* Compute all new terms.  For index i: cache[i] == a(i+1). */
        for (size_t i = cache_size; i < need; ++i) {
            if (i < 2) {
                new_cache[i] = (seq_t)1;                /* a(1), a(2) */
            } else {
                seq_t a1 = new_cache[i - 1];            /* a(i)   */
                seq_t a2 = new_cache[i - 2];            /* a(i-1) */
                new_cache[i] = a1 * a2 - a1 - a2;
            }
        }

        cache = new_cache;
        cache_size = need;
    }

    return cache[n - 1];
}

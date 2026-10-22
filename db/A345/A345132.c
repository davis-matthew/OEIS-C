/*
 * A345132  Number of (n+2) X (n+2) symmetric matrices with nonnegative
 *          integer entries, trace 0, with n rows that sum to 2, and 2 rows
 *          that sum to 1.
 * https://oeis.org/A345132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Recurrence derived from the exponential generating function
   G(x) = exp(x^2/4 - x/2) / (1 - x)^(3/2):

      a(0) = a(1) = 1
      for n ≥ 2
      a(n) = (n+1) * a(n-1) + n * a(n-2) -
             n * (n-1) / 2 * a(n-3)

   The function memoises the results in a static array so that it
   answers any call in O(n) time (each value is computed exactly
   once).
*/
#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, free, realloc */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;      /* array of a[0..cap-1] */
    static size_t cap   = 0;         /* current capacity */
    static idx_t  last  = (idx_t)-1; /* largest n with a[n] computed */

    /* ensure we have space for indices up to n */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;   /* allocation failure */

        /* copy old data if any */
        if (cache) {
            for (size_t i = 0; i < cap; ++i) new_cache[i] = cache[i];
            free(cache);
        }

        cache = new_cache;
        cap   = new_cap;
    }

    /* initialise base values if not already done */
    if (cache && last == (idx_t)-1) {
        if (cap >= 1) cache[0] = 1;
        if (cap >= 2) cache[1] = 1;
        last = (cap >= 2 ? 1 : 0);
    }

    /* compute missing values up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t term = (seq_t)(i) * cache[(size_t)(i - 1)];
        term += (seq_t)(i - 1) * cache[(size_t)(i - 2)];
        if (i >= 3) {
            seq_t factor = (seq_t)(i - 1) * (seq_t)(i - 2) / (seq_t)2;
            term -= factor * cache[(size_t)(i - 3)];
        }
        cache[(size_t)i] = term;
    }

    last = n;
    return cache[(size_t)n];
}

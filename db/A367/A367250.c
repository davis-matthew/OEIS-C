/*
 * A367250  a(n) is the number of n-digit numbers whose difference between the
 *          largest and smallest digits is equal to 9.
 * https://oeis.org/A367250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A367250 – number of n‑digit numbers whose largest and smallest
   digits differ by 9.
   Recurrence (n > 3):
        a(n) = 27*a(n-1) - 242*a(n-2) + 720*a(n-3)
   Initial values:
        a(1) = 0
        a(2) = 1
        a(3) = 35
   The implementation memoises computed values in a static array, so calls
   in arbitrary order are handled efficiently.  All operations are carried
   out in the integer type seq_t supplied by the build; for arbitrary‑
   precision builds this gives exact results for all n.  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache[1 .. cache_size] holds a(k) */
    static idx_t  cache_size = 0;

    /* If n is already cached, simply return it. */
    if (n <= cache_size)
        return cache[n];

    /* Allocate a new cache large enough for n. */
    seq_t *new_cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (new_cache == NULL)          /* allocation failed – shouldn't happen */
        return (seq_t)0;

    /* Copy any previously cached values. */
    for (idx_t i = 1; i <= cache_size; ++i)
        new_cache[i] = cache[i];

    /* Compute while extending the cache. */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i == 1)
            new_cache[1] = (seq_t)0;
        else if (i == 2)
            new_cache[2] = (seq_t)1;
        else if (i == 3)
            new_cache[3] = (seq_t)35;
        else {
            seq_t v1 = (seq_t)27 * new_cache[i - 1];
            seq_t v2 = (seq_t)242 * new_cache[i - 2];
            seq_t v3 = (seq_t)720 * new_cache[i - 3];
            new_cache[i] = v1 - v2 + v3;
        }
    }

    /* Release the old cache and store the new one. */
    free(cache);
    cache      = new_cache;
    cache_size = n;

    return cache[n];
}

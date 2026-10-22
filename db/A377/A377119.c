/*
 * A377119  a(n) = coefficient of 2^(2/3) in the expansion of (2^(1/3) +
 *          2^(2/3))^n.
 * https://oeis.org/A377119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-2) + 6*a(n-3) (n ≥ 3), a(0) = 0, a(1) = 1, a(2) = 1
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache storing already computed values */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;   /* allocated size of cache   */
    static size_t  last   = 0;   /* largest index computed yet */

    /* initialise cache on first call */
    if (!cache) {
        cap   = 3;                      /* need entries 0,1,2 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();            /* out of memory */

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        last   = 2;
        if ((size_t)n <= last) return cache[(size_t)n];
    }

    /* grow the cache when the requested index is beyond current capacity */
    if ((size_t)n >= cap) {
        size_t new_cap = cap ? cap << 1 : 3;
        while (new_cap <= (size_t)n) new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* out of memory */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing terms up to n using the recurrence */
    if ((size_t)n > last) {
        for (size_t i = last + 1; i <= (size_t)n; ++i) {
            cache[i] = (seq_t)6 * cache[i-2] + (seq_t)6 * cache[i-3];
        }
        last = (size_t)n;
    }

    return cache[(size_t)n];
}

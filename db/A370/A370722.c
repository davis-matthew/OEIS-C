/*
 * A370722  a(n) = Sum_{k=0..floor(n/7)} binomial(n-4*k,3*k).
 * https://oeis.org/A370722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) + a(n-7), with initial
 *            values a(0) = 1, a(1) = 1, a(2) = 1, a(3) = 1, a(4) = 1, a(5) =
 *            1, a(6) = 1, a(7) = 2. Offset: 0
 * Range:     u8 n <= 16, u16 n <= 28, u32 n <= 53, u64 n <= 103,
 *            u128 n <= 204, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The following function must have external linkage.  It maintains a
 * static cache so that calls may come in any order and the recurrence
 * only needs to be evaluated for the missing terms. */
seq_t a(idx_t n)
{
    /* cache[i] holds a(i) for 0 <= i < cache_size.
       Initially cache is NULL and cache_size is 0. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* If the requested value is already cached, return it immediately. */
    if (n < cache_size)
        return cache[n];

    /* Extend the cache array to size n+1.  Use malloc/realloc with an
       explicit cast so the code is valid both in C and C++. */
    idx_t new_size = n + 1;
    seq_t *new_cache;
    if (cache == NULL)
        new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    else
        new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (new_cache == NULL) {
        /* Memory allocation failed; return 0 as a sentinel. */
        return (seq_t)0;
    }
    cache = new_cache;

    /* Compute all missing terms from the previous size up to n,
       inclusive.  Use the recurrence once the base values are known. */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i <= 6) {
            cache[i] = 1;
        } else if (i == 7) {
            cache[i] = 2;
        } else {
            /* Compute using the recurrence.  All intermediate
               computations are performed in seq_t, which can be an
               integer type or an arbitrary‑precision class. */
            seq_t val = 3 * cache[i - 1];
            val -= 3 * cache[i - 2];
            val += cache[i - 3];
            val += cache[i - 7];
            cache[i] = val;
        }
    }

    cache_size = new_size;
    return cache[n];
}

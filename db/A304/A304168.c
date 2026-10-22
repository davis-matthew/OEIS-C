/*
 * A304168  a(n) = 2*3^n - 2^(n-1) (n>=1).
 * https://oeis.org/A304168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*3^n - 2^(n-1) (n>=1) Recurrence: a(1)=5, a(2)=16,
 *            a(n)=5*a(n-1)-6*a(n-2) for n>2
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1U)
        return (seq_t)0;          /* Out of range, undefined for this sequence */

    static seq_t *cache = NULL;   /* 1‑based index; cache[0] unused */
    static idx_t cache_cap = 0;   /* Allocated size (indices 0..cache_cap-1) */

    /* Initialise cache for the first two terms */
    if (cache_cap < 3) {
        cache_cap = 3;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!cache) abort();          /* Allocation failure */
        cache[1] = (seq_t)5;          /* a(1) */
        cache[2] = (seq_t)16;         /* a(2) */
    }

    /* If requested value already computed, return it */
    if (n < cache_cap)
        return cache[n];

    /* Extend cache to accommodate up to n */
    cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!cache) abort();

    /* Compute missing terms using the recurrence */
    for (idx_t i = cache_cap; i <= n; ++i) {
        cache[i] = (seq_t)5 * cache[i - 1] - (seq_t)6 * cache[i - 2];
    }
    cache_cap = n + 1;

    return cache[n];
}

/*
 * A394799  a(0) = a(1) = a(2) = 1; a(n) = a(n-1) + (n-2)*a(n-3).
 * https://oeis.org/A394799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + (n-2)*a(n-3) with a(0) = a(1) = a(2) = 1
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 24, u64 n <= 42, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t cap     = 0;   /* allocated size of cache   */
    static idx_t  computed = 0;  /* next index to compute     */

    /* Ensure the cache can hold indices 0..n and at least the 3 base values */
    size_t needed = n + 1;
    if (needed < 3) needed = 3;
    if (cap < needed) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cap   = needed;
    }

    /* Initialise the base cases once (only up to the allocated space) */
    if (computed == 0) {
        cache[0] = (seq_t)1;
        if (cap > 1) cache[1] = (seq_t)1;
        if (cap > 2) cache[2] = (seq_t)1;
        computed = (cap >= 3 ? 3 : cap);   /* first index that is still unknown */
    }

    /* Return already known value */
    if (n < computed) return cache[n];

    /* Compute missing values up to n */
    for (idx_t i = computed; i <= n; ++i) {
        seq_t term = cache[i-1] + ((seq_t)(i-2) * cache[i-3]);
        cache[i] = term;
    }
    computed = n + 1;
    return cache[n];
}

/*
 * A252717  Number of (n+2) X (6+2) 0..3 arrays with every consecutive three
 *          elements in every row and diagonal having exactly two distinct
 *          values, and in every column and antidiagonal not having exactly
 *          two distinct values, and new values 0 upwards introduced in row
 *          major order.
 * https://oeis.org/A252717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-3) - 3*a(n-4) - a(n-6) + a(n-7) for n>9
 *            Initial values: a(1)=175, a(2)=91, a(3)=110, a(4)=132, a(5)=191,
 *            a(6)=262, a(7)=324, a(8)=478, a(9)=664, a(10)=828
 * Range:     u8 n <= 5, u16 n <= 23, u32 n <= 58, u64 n <= 127, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* largest index stored  (>=1) */

    /* First call: initialise cache up to n=10 */
    if (cache == NULL) {
        /* Allocate room for index 0..10 */
        cache_cap = 10;
        cache = (seq_t*)malloc(sizeof(seq_t) * (cache_cap + 1));
        if (!cache) return (seq_t)0;          /* malloc failed: return 0 */

        cache[0] = 0;
        cache[1] = 175; /* a(1) */
        cache[2] = 91;   /* a(2) */
        cache[3] = 110;  /* a(3) */
        cache[4] = 132;  /* a(4) */
        cache[5] = 191;  /* a(5) */
        cache[6] = 262;  /* a(6) */
        cache[7] = 324;  /* a(7) */
        cache[8] = 478;  /* a(8) */
        cache[9] = 664;  /* a(9) */
        cache[10] = 828; /* a(10) */
    }

    /* If the requested index is already cached, return it */
    if (n <= cache_cap) return cache[n];

    /* Need to extend the cache up to n */
    size_t needed = (size_t)n;

    /* Reallocate cache for indices 0..n */
    seq_t *new_cache = (seq_t*)realloc(cache, sizeof(seq_t) * (needed + 1));
    if (!new_cache) return (seq_t)0;          /* realloc failed: return 0 */
    cache = new_cache;

    /* Compute the missing terms using the recurrence */
    for (size_t i = cache_cap + 1; i <= needed; ++i) {
        cache[i] =
            cache[i - 1] +
            3 * cache[i - 3] -
            3 * cache[i - 4] -
            cache[i - 6] +
            cache[i - 7];
    }

    cache_cap = needed;
    return cache[n];
}

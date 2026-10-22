/*
 * A335559  a(n) = 3*a(n-1) + 4*a(n-2) - 2*a(n-3) with a(0)=0, a(1)=1, a(2)=2.
 * https://oeis.org/A335559
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 4*a(n-2) - 2*a(n-3), a(0)=0, a(1)=1, a(2)=2
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle initial values directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static size_t len   = 0;   /* number of cached values (maximum index + 1) */
    static size_t cap   = 0;   /* allocated capacity */

    /* Initial allocation and seed for n>=3 */
    if (len == 0) {
        cap = 3;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;           /* allocation failure abort */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        len = 3;
    }

    /* Grow buffer if necessary */
    if ((size_t)n + 1 > cap) {
        size_t newCap = cap;
        while (newCap < (size_t)n + 1) newCap = newCap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, newCap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failure abort */
        cache = tmp;
        cap = newCap;
    }

    /* Compute missing values up to n */
    for (size_t i = len; i <= (size_t)n; ++i) {
        seq_t val = 3 * cache[i-1] + 4 * cache[i-2] - 2 * cache[i-3];
        cache[i] = val;
    }
    len = (size_t)n + 1;

    return cache[(size_t)n];
}

/*
 * A291142  a(n) = (1/4)*A291024(n).
 * https://oeis.org/A291142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - 4*a(n-3) - a(n-4) for n >= 4
 *            Initial values: a(0)=0, a(1)=1, a(2)=2, a(3)=6
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 49, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation; grows as needed */
    static seq_t *cache = NULL;
    static idx_t cap = 0;      /* allocated size */
    static idx_t len = 0;      /* number of elements fully computed (len = next index to compute) */

    /* initialise cache with first four terms if not yet done */
    if (cache == NULL) {
        cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache)   /* allocation failure – return a safe value */
            return (seq_t)0;
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)6;
        len   = 4;   /* next index to compute */
    }

    /* if the requested term is already cached, return it */
    if (n < len)
        return cache[n];

    /* ensure the array is large enough to hold up to index n */
    if (n >= cap) {
        idx_t newcap = cap ? cap : 4;
        while (newcap <= n)
            newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp)   /* allocation failure – return a safe value */
            return (seq_t)0;
        cache = tmp;
        cap   = newcap;
    }

    /* compute missing values up to n */
    for (idx_t i = len; i <= n; ++i) {
        if (i >= 4) {
            /* recurrence: a(i) = 4*a(i-1) - 2*a(i-2) - 4*a(i-3) - a(i-4) */
            cache[i] = 4 * cache[i-1]
                     - 2 * cache[i-2]
                     - 4 * cache[i-3]
                     -     cache[i-4];
        } else {
            /* should never happen – safety fallback */
            cache[i] = (seq_t)0;
        }
    }

    len = n + 1;   /* update the cache boundary */
    return cache[n];
}

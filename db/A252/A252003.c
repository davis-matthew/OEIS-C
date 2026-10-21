/*
 * A252003  Numbers n such that the sum of the octagonal numbers N(n) and
 *          N(n+1) is equal to the sum of the pentagonal numbers P(m) and
 *          P(m+1) for some m.
 * https://oeis.org/A252003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1155*a(n-1) - 1155*a(n-2) + a(n-3) Initial values:
 *            a(1)=232, a(2)=267920, a(3)=309179640
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache holding terms from a(1) to a(cache_sz) */
    static seq_t *cache = NULL;
    static idx_t cache_sz = 0;

    /* Return 0 for indices below the offset; actual usage starts at 1 */
    if (n < 1) {
        return (seq_t)0;
    }

    /* First call: allocate enough space for at least n and initialise the
       known first three terms. */
    if (cache == NULL) {
        idx_t init = n > 3 ? n : 3;          /* allocate at least three slots */
        cache = (seq_t *)malloc(init * sizeof(seq_t));
        if (cache == NULL) {               /* allocation failed */
            return (seq_t)0;
        }
        cache[0] = (seq_t)232;        /* a(1) */
        if (init > 1) cache[1] = (seq_t)267920;   /* a(2) */
        if (init > 2) cache[2] = (seq_t)309179640;/* a(3) */
        cache_sz = init;
    }

    /* If we don't yet have the requested term, grow the cache and compute */
    if (n > cache_sz) {
        /* Grow the cache to at least size n */
        cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (cache == NULL) {               /* allocation failed */
            return (seq_t)0;
        }
        /* Compute missing terms using the recurrence.  Cache indices:
           cache[i-1] corresponds to a(i)  (i starts at 1).  */
        for (idx_t i = cache_sz + 1; i <= n; ++i) {
            seq_t t1 = (seq_t)1155 * cache[i - 2];     /* a(i-1) */
            seq_t t2 = (seq_t)1155 * cache[i - 3];     /* a(i-2) */
            seq_t t3 = cache[i - 4];                  /* a(i-3)   */
            cache[i - 1] = t1 - t2 + t3;
        }
        cache_sz = n;
    }

    return cache[n - 1];
}

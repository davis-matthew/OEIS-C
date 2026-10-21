/*
 * A247301  Rectangular array read upwards by columns: T = T(n,k) = number of
 *          paths from (0,0) to (n,k), where 0 >= k <= 2, consisting of
 *          segments given by the vectors (1,1), (2,1), (1,-1).
 * https://oeis.org/A247301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = 2*a(n-6)+2*a(n-9) (n > 14) Initial values: a(0)=1,
 *            a(1)=0, a(2)=0, a(3)=0, a(4)=1, a(5)=0, a(6)=1, a(7)=1, a(8)=1,
 *            a(9)=1, a(10)=2, a(11)=2, a(12)=2, a(13)=4, a(14)=3
 * Range:     u8 n <= 36, u16 n <= 66, u32 n <= 123, u64 n <= 240,
 *            u128 n <= 474, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Return the n-th term of OEIS A247301 */
seq_t a(idx_t n)
{
    /* Initial terms 0 … 14 */
    static const seq_t init[] = {
        1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 2, 2, 2, 4, 3
    };
    static const size_t init_len = sizeof(init) / sizeof(init[0]);

    /* Cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* On first call, allocate cache and copy the initial terms */
    if (cache_len == 0) {
        cache_len = init_len;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;
        for (size_t i = 0; i < init_len; ++i) cache[i] = init[i];
    }

    /* if the requested index is not yet cached, extend the cache */
    if ((size_t)n >= cache_len) {
        /* new length should be exactly n+1 (since n is unsigned) */
        size_t new_len = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) return 0;  /* allocation failed */
        cache = new_cache;
        /* compute new terms from current len up to n */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            /* recurrence only applies for i >= 15 */
            if (i < init_len) {
                /* already initialized */
                continue;
            }
            seq_t t6 = cache[i - 6];
            seq_t t9 = cache[i - 9];
            cache[i] = t6 + t6 + t9 + t9;   /* 2*a(i-6)+2*a(i-9) */
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}

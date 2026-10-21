/*
 * A291037  a(n) = (1/2)*A291036(n).
 * https://oeis.org/A291037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=3, a(2)=8, a(3)=23, a(4)=66, a(5)=188, a(6)=537;
 *            a(n) = 2*a(n-1)+2*a(n-2)+2*a(n-3)-2*a(n-4)-a(n-6) for n>=7
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of valid entries in cache */

    /* Return cached value if already known */
    if (n < cache_len) {
        return cache[n];
    }

    /* Initialise cache with the first 7 terms */
    if (cache_len == 0) {
        cache_len = 7;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;      /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)23;
        cache[4] = (seq_t)66;
        cache[5] = (seq_t)188;
        cache[6] = (seq_t)537;
    }

    /* If we need more terms, extend the cache and compute them */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failed */
        cache = tmp;

        for (idx_t i = cache_len; i <= n; ++i) {
            /* recurrence: a(n) = 2*a(n-1)+2*a(n-2)+2*a(n-3)-2*a(n-4)-a(n-6) */
            cache[i] = 2 * cache[i-1] + 2 * cache[i-2] + 2 * cache[i-3]
                     - 2 * cache[i-4] - cache[i-6];
        }
        cache_len = new_len;
    }

    return cache[n];
}

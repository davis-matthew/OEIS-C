/*
 * A241073  Number of n X 2 0..2 arrays with no element equal to exactly one
 *          horizontal or vertical neighbor, with new values 0..2 introduced
 *          in row major order.
 * https://oeis.org/A241073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 8*a(n-2) - 4*a(n-3) + a(n-4) + 14*a(n-5) -
 *            8*a(n-6) for n > 7. Initial values: a(1)=1, a(2)=4, a(3)=12,
 *            a(4)=39, a(5)=131, a(6)=444, a(7)=1516. The function memoises
 *            results so repeated calls are fast
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* memoisation table; index 0 unused, cache[1] = a(1) ...  */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated entries (not including 0)   */
    static idx_t  cache_len = 0;   /* highest n for which cache[n] is valid */

    if (n == 0) return (seq_t)0;   /* undefined but harmless */

    /* grow cache to at least n+1 entries (index 0 unused) */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure; should not happen */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise first terms on first call or if cache was cleared */
    if (cache_len == 0) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)12;
        cache[4] = (seq_t)39;
        cache[5] = (seq_t)131;
        cache[6] = (seq_t)444;
        cache[7] = (seq_t)1516;
        cache_len = 7;
    }

    if (n <= cache_len) return cache[n];

    /* constants as seq_t */
    const seq_t six     = (seq_t)6;
    const seq_t eight   = (seq_t)8;
    const seq_t four    = (seq_t)4;
    const seq_t fourteen= (seq_t)14;

    /* compute new terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t val;
        if (i >= 8) {  /* recurrence valid from n>7 */
            val = six   * cache[i-1]
                - eight * cache[i-2]
                - four  * cache[i-3]
                + cache[i-4]
                + fourteen * cache[i-5]
                - eight  * cache[i-6];
        } else {
            /* for i==8 compute using recurrence, otherwise values are pre‑seeded */
            val = six   * cache[i-1]
                - eight * cache[i-2]
                - four  * cache[i-3]
                + cache[i-4]
                + fourteen * cache[i-5]
                - eight  * cache[i-6];
        }
        cache[i] = val;
    }
    cache_len = n;

    return cache[n];
}

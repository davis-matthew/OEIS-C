/*
 * A352008  a(0) = 1; a(n) = Sum_{k=0..floor((n-1)/2)} 4^k * a(k) *
 *          a(n-2*k-1).
 * https://oeis.org/A352008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊(n-1)/2⌋} 4^k * a(k) * a(n-2k-1)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 36, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Base: a(0) = 1, a(1) = 1, a(2) = 1 (obtained by the recurrence) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;      /* array of computed values   */
    static size_t cache_cap = 0;     /* allocated size of cache   */
    static idx_t  highest = (idx_t)0;/* highest index computed    */

    /* Ensure the cache exists and holds a[0] = 1 */
    if (cache == NULL) {
        size_t init_cap = (size_t)1;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        cache_cap = init_cap;
        cache[0] = (seq_t)1;
        highest = 0;
    }

    /* size_t conversion safe because n is unsigned */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = needed * 2;           /* grow to reduce reallocs */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing values up to n (in any call order) */
    for (idx_t i = highest + 1; i <= n; ++i) {
        if (i == 0) {                         /* already set, but keep guard */
            cache[0] = (seq_t)1;
            continue;
        }

        seq_t sum  = (seq_t)0;
        seq_t pow4 = (seq_t)1;                /* 4^k for current k */
        idx_t  maxk = (i - (idx_t)1) / (idx_t)2;

        for (idx_t k = (idx_t)0; k <= maxk; ++k) {
            /* term = 4^k * a(k) * a(i-2k-1) */
            sum += pow4 * cache[k] * cache[i - ((idx_t)2 * k) - (idx_t)1];
            pow4 *= (seq_t)4;
        }
        cache[i] = sum;
    }
    highest = n;

    return cache[n];
}

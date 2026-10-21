/*
 * A268289  a(0)=0; thereafter a(n) = a(n-1) - A037861(n).
 * https://oeis.org/A268289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, hear, nice, look
 * Formula:   a(0)=0; for n>0, a(2*n)=a(n)+a(n-1); a(2*n+1)=2*a(n)+1
 * Range:     u8 n <= 425, u16 n <= 8054, u32 n <= 8054, u64 n <= 8054,
 *            u128 n <= 8054, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the recursive definition using memoised odd/even formulas. */
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* initialise cache with a(0) = 0 */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) return 0;          /* allocation failure → 0 (fallback) */
        cache[0] = 0;
        cache_len = 1;
    }

    /* compute missing values up to n */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) return 0;          /* allocation failure → 0 (fallback) */
        cache = tmp;
        for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
            if ((i & 1) == 0) {            /* even */
                idx_t half = i >> 1;
                cache[i] = cache[half] + cache[half - 1];
            } else {                       /* odd */
                idx_t k = (i - 1) >> 1;
                cache[i] = 2 * cache[k] + 1;
            }
        }
        cache_len = new_len;
    }

    return cache[n];
}

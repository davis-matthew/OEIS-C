/*
 * A275436  Sum of the asymmetry degrees of all 00-avoiding binary words of
 *          length n.
 * https://oeis.org/A275436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+3*a(n-2)-2*a(n-3)-2*a(n-5)-3*a(n-6)+a(n-7)+a(n-8)
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 41, u64 n <= 85, u128 n <= 176,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Recursive formula based cache.  Values are stored up to the
 * current maximum n computed.  The cache grows lazily and
 * reuses already computed terms. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of elements stored */

    /* size needed */
    idx_t need = n + 1;           /* indices 0..n inclusive */

    /* allocate or grow cache if necessary */
    if (cache == NULL) {
        /* first allocation: at least need, but also at least 9 for base data */
        idx_t init = need > 9 ? need : 9;
        cache = (seq_t*)malloc(init * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure: return 0 */
        cache_len = init;
    } else if (need > cache_len) {
        /* grow: double size or just enough */
        idx_t new_len = cache_len * 2;
        if (new_len < need) new_len = need;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure: return 0 */
        cache = tmp;
        cache_len = new_len;
    }

    /* Ensure base values are set (indices 0..8) */
    if (cache_len > 0) {
        if (need > 0) cache[0] = (seq_t)0;
        if (need > 1) cache[1] = (seq_t)0;
        if (need > 2) cache[2] = (seq_t)2;
        if (need > 3) cache[3] = (seq_t)2;
        if (need > 4) cache[4] = (seq_t)8;
        if (need > 5) cache[5] = (seq_t)10;
        if (need > 6) cache[6] = (seq_t)30;
        if (need > 7) cache[7] = (seq_t)40;
        if (need > 8) cache[8] = (seq_t)100;
    }

    /* Compute new terms up to n */
    for (idx_t i = 0; i < need; ++i) {
        if (i <= 8) continue;   /* base values already set */
        /* recurrence:
         * a(i) = a(i-1) + 3*a(i-2) - 2*a(i-3) - 2*a(i-5) - 3*a(i-6) + a(i-7) + a(i-8)
         */
        cache[i] =
            cache[i-1] +
            cache[i-2] * (seq_t)3 -
            cache[i-3] * (seq_t)2 -
            cache[i-5] * (seq_t)2 -
            cache[i-6] * (seq_t)3 +
            cache[i-7] +
            cache[i-8];
    }

    return cache[n];
}

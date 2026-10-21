/*
 * A291246  p-INVERT of (0,1,0,1,0,1,...), where p(S) = 1 - 6 S + S^2.
 * https://oeis.org/A291246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) + a(n-2) - 6*a(n-3) - a(n-4) for n >= 5; Initial
 *            values: a(0)=6, a(1)=35, a(2)=210, a(3)=1259, a(4)=7548
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (cache == NULL) {
        /* Initial allocation large enough for at least the first 5 terms. */
        cache_len = (n < 5 ? 5 : n + 1);
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);

        cache[0] = 6;
        cache[1] = 35;
        cache[2] = 210;
        cache[3] = 1259;
        cache[4] = 7548;

        /* Compute terms up to n (if n >= 5). */
        for (idx_t i = 5; i <= n; ++i) {
            seq_t d1 = cache[i - 1] - cache[i - 3];
            d1 *= (seq_t)6;
            seq_t d2 = cache[i - 2] - cache[i - 4];
            cache[i] = d1 + d2;
        }
    } else if (n + 1 > cache_len) {
        idx_t old_len = cache_len;
        cache_len = n + 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_len);

        /* Compute newly required terms. */
        for (idx_t i = old_len; i <= n; ++i) {
            seq_t d1 = cache[i - 1] - cache[i - 3];
            d1 *= (seq_t)6;
            seq_t d2 = cache[i - 2] - cache[i - 4];
            cache[i] = d1 + d2;
        }
    }

    return cache[n];
}

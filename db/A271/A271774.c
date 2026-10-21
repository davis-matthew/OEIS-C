/*
 * A271774  a(1) = 1, then a(n) is the maximum of all 0 < m < n for which a(m)
 *          divides n.
 * https://oeis.org/A271774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max{ m | 0 < m < n and a(m) divides n }, with a(1) = 1
 * Range:     u8 n <= 257, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static memoization cache */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;  /* highest index computed (>=0) */

    /* Grow cache if necessary */
    if (n > cache_len) {
        size_t new_len = n;
        if (cache == NULL) {
            cache = (seq_t *)malloc((new_len + 1) * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, (new_len + 1) * sizeof(seq_t));
        }
        /* Compute new entries */
        for (idx_t cur = cache_len + 1; cur <= new_len; ++cur) {
            if (cur == 1) {
                cache[1] = 1;   /* a(1) = 1 by definition */
                continue;
            }
            seq_t best = 0;
            for (idx_t m = cur - 1; m > 0; --m) {
                if (((seq_t)cur) % cache[m] == 0) {
                    best = m;
                    break;          /* largest m found */
                }
            }
            cache[cur] = best;
        }
        cache_len = new_len;
    }
    return cache[n];
}

/*
 * A250975  Number of (n+1) X (1+1) 0..2 arrays with no 2 X 2 subblock having
 *          the sum of its diagonal elements less than the absolute difference
 *          of its antidiagonal elements.
 * https://oeis.org/A250975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 10*a(n-2) + 13*a(n-3) - 6*a(n-4) + a(n-5),
 *            a(1) = 71, a(2) = 562, a(3) = 4459, a(4) = 35381, a(5) = 280728
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;      /* cache[0] unused, cache[1] = a(1), ... */
    static idx_t cache_len = 0;      /* current size of cache (elements 0..cache_len-1) */

    /* Ensure the cache contains terms up to index n */
    if (cache_len <= n) {
        size_t new_len = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;

        /* Initialize newly added elements */
        for (idx_t i = cache_len; i <= n; ++i) {
            switch (i) {
                case 0:
                    cache[0] = 0;
                    break;
                case 1:
                    cache[1] = (seq_t)71;
                    break;
                case 2:
                    cache[2] = (seq_t)562;
                    break;
                case 3:
                    cache[3] = (seq_t)4459;
                    break;
                case 4:
                    cache[4] = (seq_t)35381;
                    break;
                case 5:
                    cache[5] = (seq_t)280728;
                    break;
                default: {
                    seq_t t1 = cache[i-1] * (seq_t)9;
                    seq_t t2 = cache[i-2] * (seq_t)10;
                    seq_t t3 = cache[i-3] * (seq_t)13;
                    seq_t t4 = cache[i-4] * (seq_t)6;
                    seq_t sum_pos = t1 + t3 + cache[i-5];
                    seq_t sum_neg = t2 + t4;
                    cache[i] = sum_pos - sum_neg;  /* sum_pos >= sum_neg for all n >= 6 */
                }
            }
        }
        cache_len = new_len;
    }

    return cache[n];
}

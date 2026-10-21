/*
 * A291183  p-INVERT of the positive integers, where p(S) = 1 - 4*S + 2*S^2.
 * https://oeis.org/A291183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=4, a(1)=22, a(2)=116, a(3)=608,
 *            a(n)=8*a(n-1)-16*a(n-2)+8*a(n-3)-a(n-4) for n>=4
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static idx_t cache_len = 0;
    static seq_t *cache = NULL;

    /* Initialisation of the first four terms */
    if (cache_len == 0) {
        cache_len = 4;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) abort();   /* out of memory */
        cache[0] = (seq_t)4;
        cache[1] = (seq_t)22;
        cache[2] = (seq_t)116;
        cache[3] = (seq_t)608;
    }

    /* Grow cache if necessary and compute missing terms */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();   /* out of memory */
        cache = tmp;

        for (idx_t i = cache_len; i <= n; ++i) {
            /* i >= 4 by construction of the loop */
            cache[i] = 8 * cache[i-1]
                     - 16 * cache[i-2]
                     + 8 * cache[i-3]
                     - cache[i-4];
        }
        cache_len = new_len;
    }

    return cache[n];
}

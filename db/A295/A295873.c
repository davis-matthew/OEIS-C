/*
 * A295873  Number of permutations of length n which avoid the patterns 1342,
 *          2413, 3124 and 3142.
 * https://oeis.org/A295873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 16*a(n-2) + 14*a(n-3) - 5*a(n-4) + a(n-5);
 *            Initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=6, a(4)=20
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 38, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_max = 0;

    size_t idx = (size_t)n;

    if (cache == NULL) {
        cache_len = 5;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;                   /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)20;
        cache_max = 4;
    }

    if (idx <= 4) return cache[idx];

    if (idx > cache_max) {
        /* Ensure the cache is large enough. */
        if (idx >= cache_len) {
            size_t new_len = cache_len;
            while (new_len <= idx) new_len <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
            if (!tmp) return 0;                 /* allocation failed */
            cache = tmp;
            cache_len = new_len;
        }

        /* Compute the sequence from cache_max+1 up to idx. */
        for (size_t i = cache_max + 1; i <= idx; ++i) {
            seq_t pos = cache[i - 1] * (seq_t)7
                       + cache[i - 3] * (seq_t)14
                       + cache[i - 5];
            seq_t neg = cache[i - 2] * (seq_t)16
                       + cache[i - 4] * (seq_t)5;
            cache[i] = pos - neg;               /* pos >= neg for all n */
        }
        cache_max = idx;
    }

    return cache[idx];
}

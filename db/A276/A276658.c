/*
 * A276658  Tribonacci-like sequence a(n) = a(n-1) + a(n-2) + a(n-3) for n >=
 *          3, with a(0) = 1, a(1) = 2, a(2) = 0.
 * https://oeis.org/A276658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=0, a(n)=a(n-1)+a(n-2)+a(n-3) for n≥3
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_used = 0;

    /* Ensure the array holds indices 0..n */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) { /* reallocation failed – fall back to malloc */
            tmp = (seq_t *)malloc(needed * sizeof(seq_t));
        }
        cache = tmp;
        cache_cap = needed;
    }

    /* Initialize base values once their slots exist */
    if (cache_used < 1 && cache_cap > 0) {
        cache[0] = (seq_t)1;
        cache_used = 1;
    }
    if (cache_used < 2 && cache_cap > 1) {
        cache[1] = (seq_t)2;
        cache_used = 2;
    }
    if (cache_used < 3 && cache_cap > 2) {
        cache[2] = (seq_t)0;
        cache_used = 3;
    }

    /* Compute any missing terms up to n */
    if (needed > cache_used) {
        for (size_t i = cache_used; i < needed; ++i) {
            cache[i] = cache[i - 1] + cache[i - 2] + cache[i - 3];
        }
        cache_used = needed;
    }

    return cache[(size_t)n];
}

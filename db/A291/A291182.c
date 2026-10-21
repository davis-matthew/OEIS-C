/*
 * A291182  p-INVERT of the positive integers, where p(S) = 1 - 3*S + 2*S^2.
 * https://oeis.org/A291182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 14*a(n-2) + 7*a(n-3) - a(n-4) Initial values:
 *            a(0)=3, a(1)=13, a(2)=52, a(3)=203
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t max_computed = 0;

    /* size needed for index n (0‑based) */
    size_t needed = (size_t)n + 1;

    if (cache_size == 0) {
        /* at least 4 entries for the initial terms */
        cache_size = (needed > 4 ? needed : 4);
        cache = (seq_t*) calloc(cache_size, sizeof(seq_t));
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)13;
        cache[2] = (seq_t)52;
        cache[3] = (seq_t)203;
        max_computed = 3;
    } else if (needed > cache_size) {
        size_t new_size = cache_size;
        while (new_size < needed) new_size <<= 1;
        seq_t *tmp = (seq_t*) realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed */
        cache = tmp;
        cache_size = new_size;
    }

    if ((size_t)n > max_computed) {
        for (size_t i = max_computed + 1; i <= (size_t)n; ++i) {
            seq_t t = cache[i-1] * 7;
            t -= cache[i-2] * 14;
            t += cache[i-3] * 7;
            t -= cache[i-4];
            cache[i] = t;
        }
        max_computed = (size_t)n;
    }

    return cache[(size_t)n];
}

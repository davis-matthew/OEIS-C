/*
 * A320469  a(n) = 3*a(n-1) + 10*a(n-2), n >= 2; a(0)=1, a(1)=1.
 * https://oeis.org/A320469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 10*a(n-2), a(0) = 1, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* Ensure cache is initialized with the first two terms. */
    if (cache_cap == 0) {
        cache_cap = 2;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
    }

    /* Grow cache if needed. */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < n + 1)
            new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* reallocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute remaining terms up to n. */
    for (idx_t i = 2; i <= n; ++i) {
        cache[i] = (seq_t)3 * cache[i - 1] + (seq_t)10 * cache[i - 2];
    }

    return cache[n];
}

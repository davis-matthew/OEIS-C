/*
 * A291398  p-INVERT of (1,1,0,0,0,0,...), where p(S) = 1 - S^2 - S^3.
 * https://oeis.org/A291398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 1, a(2) = 3, a(3) = 5, a(4) = 9, a(5) = 19,
 *            a(6) = 39; for n ≥ 7: a(n) = a(n-2) + 3*a(n-3) + 4*a(n-4) +
 *            3*a(n-5) + a(n-6)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache      = NULL; /* pointer to array of seq_t */
    static size_t cache_len  = 0;    /* number of terms currently stored */
    static size_t cache_cap  = 0;    /* allocated capacity */

    /* initial terms of the sequence */
    static const seq_t init[7] = {0, 1, 3, 5, 9, 19, 39};

    /* --- initialise cache with the first 7 terms if this is the first call --- */
    if (cache_len == 0) {
        cache_cap = 7;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 0; i < 7; ++i)
            cache[i] = init[i];
        cache_len = 7;
    }

    /* --- return value if it is already computed --- */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* --- extend the cache if necessary --- */
    size_t needed = (size_t)n + 1;
    if (cache_cap < needed) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < needed)
            new_cap = needed;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* --- compute all missing terms up to index n --- */
    for (size_t i = cache_len; i <= n; ++i) {
        cache[i] =
            cache[i-2] +
            (seq_t)3 * cache[i-3] +
            (seq_t)4 * cache[i-4] +
            (seq_t)3 * cache[i-5] +
            cache[i-6];
    }
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}

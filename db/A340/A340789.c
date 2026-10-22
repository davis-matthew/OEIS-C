/*
 * A340789  a(n) = (n!)^2 * Sum_{k=1..n} (-1)^(k+1) / (k!)^2.
 * https://oeis.org/A340789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0 a(n) = n^2 * a(n-1) - (-1)^n for n >= 1 (Equivalently
 *            a(n) = n^2 * a(n-1) + (n odd ? +1 : -1))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>   /* for compatibility with the build system */

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Ensure base case a(0) is stored */
    if (cache_size == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 0;
        cache_size = 1;
    }

    /* Grow the cache if needed */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;

        for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
            seq_t prod = ((seq_t)i) * ((seq_t)i) * cache[i - 1];
            if (i & 1ULL)          /* n odd: add 1 */
                cache[i] = prod + 1;
            else                  /* n even: subtract 1 */
                cache[i] = prod - 1;
        }
        cache_size = new_size;
    }

    return cache[n];
}

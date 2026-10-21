/*
 * A249864  A special solution of X(n)^2 - 120*Y(n)^2 = 7^(2*n), n >= 0. The
 *          present sequence gives the X values.
 * https://oeis.org/A249864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 13; a(n) = 26*a(n-1) - 49*a(n-2), n >= 2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Initialisation */
    if (cache == NULL) {
        size_t init_cap = 16;
        cache_cap = init_cap;
#ifdef __cplusplus
        cache = new seq_t[cache_cap];
#else
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
#endif
        /* First two terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)13;
        cache_len = 2;
    }

    /* Return already computed value if available */
    if ((size_t)n < cache_len) {
        return cache[n];
    }

    /* Ensure enough capacity for index n */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap <= (size_t)n) {
            new_cap <<= 1;
        }
#ifdef __cplusplus
        seq_t *new_cache = new seq_t[new_cap];
        for (size_t i = 0; i < cache_len; ++i) {
            new_cache[i] = cache[i];
        }
        delete[] cache;
        cache = new_cache;
#else
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* realloc preserves existing contents */
        cache = new_cache;
#endif
        cache_cap = new_cap;
    }

    /* Compute terms up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t val = (seq_t)26 * cache[i - 1] - (seq_t)49 * cache[i - 2];
        cache[i] = val;
    }
    cache_len = (size_t)n + 1;
    return cache[n];
}

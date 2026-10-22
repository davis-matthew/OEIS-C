/*
 * A322461  Sum of n-th powers of the roots of x^3 + 8*x^2 + 5*x - 1.
 * https://oeis.org/A322461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=3, a(1)=-8, a(2)=54 a(n) = -8*a(n-1) - 5*a(n-2) + a(n-3)
 *            for n > 2 (recurrence derived from the cubic x^3 + 8x^2 + 5x -
 *            1)
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 10, i64 n <= 21, i128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <vector>
#endif

/* Computes a(n) for the OEIS sequence A322461 */
seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++17 build: use std::vector to hold big‑integer objects */
    static std::vector<seq_t> cache(3);
    static bool initialized = false;
    if (!initialized) {
        cache[0] = static_cast<seq_t>(3);
        cache[1] = static_cast<seq_t>(-8);
        cache[2] = static_cast<seq_t>(54);
        initialized = true;
    }
    if ((size_t)n >= cache.size()) {
        size_t old = cache.size();
        cache.resize(n + 1);
        for (size_t i = old; i <= (size_t)n; ++i) {
            cache[i] = -static_cast<seq_t>(8) * cache[i-1]
                       - static_cast<seq_t>(5) * cache[i-2]
                       + cache[i-3];
        }
    }
    return cache[(size_t)n];
#else
    /* C11 build: use a dynamically allocated array of builtin integers */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    if (!cache) {
        cache_size = 3;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) return (seq_t)0;        /* allocation failed */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)-8;
        cache[2] = (seq_t)54;
    }

    if ((size_t)n >= cache_size) {
        size_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp) return (seq_t)0;          /* allocation failed */
        cache = tmp;
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            cache[i] = - (seq_t)8 * cache[i-1]
                       - (seq_t)5 * cache[i-2]
                       + cache[i-3];
        }
        cache_size = new_size;
    }
    return cache[(size_t)n];
#endif
}

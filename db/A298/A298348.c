/*
 * A298348  a(n) = a(n-1) + a(n-2) + a([(n+1)/2]), where a(0) = 1, a(1) = 1,
 *          a(2) = 1.
 * https://oeis.org/A298348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a((n+1)/2) Initial values: a(0) = 1,
 *            a(1) = 1, a(2) = 1 Memoized computation with a static cache that
 *            grows as needed
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 91, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifdef __cplusplus
#  include <vector>
#  include <cstddef>
#endif

/* Helper to produce the value 1 in a way that works for both
   integer types (C) and arbitrary‑precision types (C++). */
static inline seq_t one_value(void) {
#ifdef __cplusplus
    return seq_t(1);
#else
    return (seq_t)1;
#endif
}

seq_t a(idx_t n) {
#ifdef __cplusplus
    /* C++ implementation using std::vector for type flexibility. */
    static std::vector<seq_t> cache;
    if (cache.empty()) {
        cache.resize(3, one_value());
    }
    if (cache.size() <= (size_t)n) {
        cache.resize(n + 1);
        for (size_t i = 3; i <= (size_t)n; ++i) {
            size_t half = (i + 1) / 2;
            cache[i] = cache[i - 1] + cache[i - 2] + cache[half];
        }
    }
    return cache[(size_t)n];
#else
    /* C implementation that works when seq_t is an unsigned integer type. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;     /* allocated capacity */
    static size_t filled = 0;    /* number of entries initialized */

    /* Initial allocation for the first three terms. */
    if (filled == 0) {
        size_t initial = 3;
        cache = (seq_t *)malloc(initial * sizeof(seq_t));
        if (!cache) abort();                     /* OOM fallback */
        cap = initial;
        cache[0] = cache[1] = cache[2] = one_value();
        filled = 3;
    }

    /* Ensure capacity for indices up to n (inclusive). */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap * 2;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                     /* OOM fallback */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute any missing terms up to n. */
    if ((size_t)n + 1 > filled) {
        for (size_t i = filled; i <= (size_t)n; ++i) {
            size_t half = (i + 1) / 2;
            cache[i] = cache[i - 1] + cache[i - 2] + cache[half];
        }
        filled = (size_t)n + 1;
    }

    return cache[(size_t)n];
#endif
}

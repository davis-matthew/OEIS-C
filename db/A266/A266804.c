/*
 * A266804  Coefficient of x^0 in the minimal polynomial of the continued
 *          fraction [1^n,sqrt(6),1,1,...], where 1^n means n ones.
 * https://oeis.org/A266804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + 15*a(n-2) - 15*a(n-3) - 5*a(n-4) + a(n-5);
 *            Initial values: a(0)=19, a(1)=19, a(2)=361, a(3)=1795,
 *            a(4)=14011
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#ifdef __cplusplus
#include <vector>
#endif
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Guard for one-time initialization */
    static bool initialized = false;

#ifdef __cplusplus
    static std::vector<seq_t> cache;
    static idx_t max_n = 0;
#else
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t max_n = 0;
#endif

    if (!initialized) {
        /* Pre‑compute the first five terms */
#ifdef __cplusplus
        cache.reserve(64);
        cache.push_back((seq_t)19);
        cache.push_back((seq_t)19);
        cache.push_back((seq_t)361);
        cache.push_back((seq_t)1795);
        cache.push_back((seq_t)14011);
#else
        /* Allocate an initial block large enough for the first terms */
        cap = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)19;
        cache[1] = (seq_t)19;
        cache[2] = (seq_t)361;
        cache[3] = (seq_t)1795;
        cache[4] = (seq_t)14011;
#endif
        max_n = 4;
        initialized = true;
    }

    if (n <= max_n) {
#ifdef __cplusplus
        return cache[(size_t)n];
#else
        return cache[(size_t)n];
#endif
    }

#ifndef __cplusplus
    /* Ensure the C array is large enough for the new values */
    if (n + 1 > cap) {
        size_t needed = (size_t)n + 1;
        size_t new_cap = cap ? cap * 2 : 64;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = new_cap;
    }
#endif

    /* Compute missing terms one by one */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t val = 5 * cache[(size_t)(i - 1)] +
                    15 * cache[(size_t)(i - 2)] -
                    15 * cache[(size_t)(i - 3)] -
                    5  * cache[(size_t)(i - 4)] +
                    cache[(size_t)(i - 5)];
#ifdef __cplusplus
        cache.push_back(val);
#else
        cache[(size_t)i] = val;
#endif
    }
    max_n = n;

    return cache[(size_t)n];
}

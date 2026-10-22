/*
 * A390045  Expansion of 1 / ((1-x)^5 - x^6).
 * https://oeis.org/A390045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + a(n-5) +
 *            a(n-6) Initial values: a(0)=1, a(1)=5, a(2)=15, a(3)=35,
 *            a(4)=70, a(5)=126
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 32, u64 n <= 68, u128 n <= 138,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#else
#include <stdlib.h>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ implementation – use std::vector for dynamic storage */
    static std::vector<seq_t> cache;
    if (cache.empty()) {
        cache.reserve(6);
        cache.push_back((seq_t)1);   /* a(0) */
        cache.push_back((seq_t)5);   /* a(1) */
        cache.push_back((seq_t)15);  /* a(2) */
        cache.push_back((seq_t)35);  /* a(3) */
        cache.push_back((seq_t)70);  /* a(4) */
        cache.push_back((seq_t)126); /* a(5) */
    }

    if ((idx_t)cache.size() > n)
        return cache[(size_t)n];

    /* Compute up to the requested index */
    for (size_t i = cache.size(); i <= (size_t)n; ++i) {
        seq_t val = 5 * cache[i-1]
                  - 10 * cache[i-2]
                  + 10 * cache[i-3]
                  - 5 * cache[i-4]
                  + cache[i-5]
                  + cache[i-6];
        cache.push_back(val);
    }
    return cache[(size_t)n];
#else
    /* C implementation – use malloc/realloc for dynamic array */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    if (cache_len == 0) {
        const size_t init_len = 6;
        cache = (seq_t *)malloc(init_len * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)5;   /* a(1) */
        cache[2] = (seq_t)15;  /* a(2) */
        cache[3] = (seq_t)35;  /* a(3) */
        cache[4] = (seq_t)70;  /* a(4) */
        cache[5] = (seq_t)126; /* a(5) */
        cache_len = init_len;
    }

    if (n < cache_len)
        return cache[(size_t)n];

    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;

    /* Compute up to the requested index */
    for (size_t i = cache_len; i < new_len; ++i) {
        cache[i] = 5 * cache[i-1]
                 - 10 * cache[i-2]
                 + 10 * cache[i-3]
                 - 5 * cache[i-4]
                 + cache[i-5]
                 + cache[i-6];
    }
    cache_len = new_len;
    return cache[(size_t)n];
#endif
}

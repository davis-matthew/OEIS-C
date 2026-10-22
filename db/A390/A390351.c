/*
 * A390351  Upper (1/2)-midsequence of F(n) and F(n+4), where F = A000045
 *          (Fibonacci numbers); see Comments.
 * https://oeis.org/A390351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A390351: a(n) = a(n-1)+a(n-2)+a(n-3)-a(n-4)-a(n-5),
   initial values a(0)=2, a(1)=3, a(2)=5, a(3)=8, a(4)=12 */

#ifdef __cplusplus
#include <vector>
#include <algorithm>
#endif
#include <stdlib.h>

#ifndef __cplusplus
/* C implementation – uses malloc/realloc */
static seq_t *cache = NULL;
static idx_t   cache_len = 0;
#else
/* C++ implementation – uses std::vector */
static std::vector<seq_t> cache;
#endif

seq_t a(idx_t n) {
#ifdef __cplusplus
    /* C++ version – vector automatically handles construction */
    const size_t needed = (size_t)(n + 1);
    const size_t old_size = cache.size();

    /* Initialise the first five terms on the first call */
    if (cache.empty()) {
        cache.resize(std::max<size_t>(5, needed));
        if (cache.size() >= 1) cache[0] = 2;
        if (cache.size() >= 2) cache[1] = 3;
        if (cache.size() >= 3) cache[2] = 5;
        if (cache.size() >= 4) cache[3] = 8;
        if (cache.size() >= 5) cache[4] = 12;
    }

    /* Grow the vector if necessary */
    if (needed > cache.size())
        cache.resize(needed);

    /* Compute new terms only */
    const size_t start = (old_size > 5) ? old_size : 5;
    for (size_t i = start; i < cache.size(); ++i) {
        cache[i] = cache[i-1] + cache[i-2] + cache[i-3]
                 - cache[i-4] - cache[i-5];
    }

    return cache[(size_t)n];
#else
    /* C version – manual memory management */
    if (cache != NULL && n < cache_len)
        return cache[n];

    /* First call or need to grow */
    if (cache == NULL) {
        const size_t init_len = (size_t)((n + 1) > 5 ? (n + 1) : 5);
        cache = (seq_t*)malloc(init_len * sizeof(seq_t));
        if (!cache) return 0;                /* allocation failure */

        /* Seed first five terms */
        cache[0] = 2; cache[1] = 3; cache[2] = 5;
        cache[3] = 8; cache[4] = 12;
        cache_len = init_len;

        /* Compute any additional terms immediately */
        if (init_len > 5) {
            for (idx_t i = 5; i < init_len; ++i) {
                cache[i] = cache[i-1] + cache[i-2] + cache[i-3]
                         - cache[i-4] - cache[i-5];
            }
        }

    } else if (n >= cache_len) {
        const size_t new_len = (size_t)((n + 1) > 5 ? (n + 1) : 5);
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;                /* allocation failure */

        cache = tmp;

        /* Compute new terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = cache[i-1] + cache[i-2] + cache[i-3]
                     - cache[i-4] - cache[i-5];
        }
        cache_len = new_len;
    }

    return cache[n];
#endif
}

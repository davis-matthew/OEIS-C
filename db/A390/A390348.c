/*
 * A390348  Lower (1/2)-midsequence of Fibonacci numbers (A000045) and
 *          tribonacci numbers (A000213); see Comments.
 * https://oeis.org/A390348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 4*a(n-4) - 2*a(n-5) + a(n-6) +
 *            2*a(n-7) + a(n-8) with (a(0),...,a(7)) = (0, 1, 1, 2, 4, 7, 12,
 *            22)
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#endif

/* ---------- storage for the recurrence ---------- */

#ifdef __cplusplus
/* C++ build – use std::vector for automatic construction */
static std::vector<seq_t> cache;
#else
/* C build – raw allocation of seq_t objects (primitive types only) */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_alloc = 0;
#endif

/* ---------- initialise first eight terms ---------- */

static void init_cache(void)
{
#ifdef __cplusplus
    if (!cache.empty())
        return;
    cache.resize(8);
    cache[0] = 0; cache[1] = 1; cache[2] = 1; cache[3] = 2;
    cache[4] = 4; cache[5] = 7; cache[6] = 12; cache[7] = 22;
#else
    if (cache)
        return;
    cache_alloc = 8;
    cache = (seq_t *)malloc(cache_alloc * sizeof(seq_t));
    if (!cache) { /* out of memory – abort in a suitable way */
        abort();
    }
    cache_len = 8;
    cache[0] = 0; cache[1] = 1; cache[2] = 1; cache[3] = 2;
    cache[4] = 4; cache[5] = 7; cache[6] = 12; cache[7] = 22;
#endif
}

/* ---------- comprehensive recurrence implementation ---------- */

seq_t a(idx_t n)
{
    init_cache();

    /* use unsigned size_t for indexing – n <= 2^64-1 */
    size_t N = (size_t)n;

#ifdef __cplusplus
    if (N < cache.size())
        return cache[N];

    cache.resize(N + 1);
    for (size_t i = 8; i <= N; ++i) {
        cache[i] = ( seq_t)2 * cache[i-1]
                 +           cache[i-2]
                 - ( seq_t)4 * cache[i-4]
                 - ( seq_t)2 * cache[i-5]
                 +           cache[i-6]
                 + ( seq_t)2 * cache[i-7]
                 +           cache[i-8];
    }
    return cache[N];
#else
    if (N < cache_len)
        return cache[N];

    /* grow the raw array if necessary */
    if (N + 1 > cache_alloc) {
        size_t new_alloc = cache_alloc ? cache_alloc : 8;
        while (new_alloc <= N)
            new_alloc <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
        if (!new_cache) {
            abort();  /* allocation failed */
        }
        cache = new_cache;
        cache_alloc = new_alloc;
    }

    for (size_t i = cache_len; i <= N; ++i) {
        cache[i] = (seq_t)2 * cache[i-1]
                 +          cache[i-2]
                 - (seq_t)4 * cache[i-4]
                 - (seq_t)2 * cache[i-5]
                 +          cache[i-6]
                 + (seq_t)2 * cache[i-7]
                 +          cache[i-8];
    }
    cache_len = N + 1;
    return cache[N];
#endif
}

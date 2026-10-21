/*
 * A253353  Number of (4+1) X (n+1) 0..1 arrays with every 2 X 2 subblock
 *          diagonal maximum minus antidiagonal maximum nondecreasing
 *          horizontally, vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A253353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence for A253353
   a(n) = 18*a(n-1) -73*a(n-2) -256*a(n-3) +2060*a(n-4) -1216*a(n-5)
          -12612*a(n-6) +18224*a(n-7) +20992*a(n-8) -40064*a(n-9) 
          -9984*a(n-10) +24576*a(n-11)      for n > 15
   Initial values (n = 1 .. 15) are provided explicitly below. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static const seq_t init[15] = {
    335ULL,
    3192ULL,
    30920ULL,
    305872ULL,
    3006936ULL,
    29746140ULL,
    294030700ULL,
    2909622092ULL,
    28790285676ULL,
    284932211532ULL,
    2819926224684ULL,
    27909384512908ULL,
    276225964612076ULL,
    2733896477818444ULL,
    27058289111384492ULL
};

#if defined(__cplusplus)
#include <vector>
static std::vector<seq_t> cache;
#else
static seq_t *cache = NULL;
static size_t cache_cap = 0;
#endif
static size_t cache_computed = 0;

static void ensure_cache_capacity(size_t need)
{
#if defined(__cplusplus)
    if (cache.size() < need)
        cache.resize(need);
#else
    if (cache_cap < need) {
        size_t new_cap = need;
        if (new_cap < 64) new_cap = 64;
        if (cache_cap > 0 && new_cap < cache_cap * 2) new_cap = cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
#endif
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n < 1 */

    /* Ensure memoisation storage can hold n terms */
    ensure_cache_capacity((size_t)n);

    /* Fill missing values up to n */
    for (size_t i = cache_computed + 1; i <= (size_t)n; ++i) {
        seq_t val;
        if (i <= 15) {
            val = init[i - 1];
        } else {
            /* 0‑based indexing: cache[k] is a(k+1) */
            size_t idx = i - 1;             /* index of a(i) in cache */
            val = (seq_t)18  * cache[idx - 1]
                - (seq_t)73  * cache[idx - 2]
                - (seq_t)256 * cache[idx - 3]
                + (seq_t)2060 * cache[idx - 4]
                - (seq_t)1216 * cache[idx - 5]
                - (seq_t)12612 * cache[idx - 6]
                + (seq_t)18224 * cache[idx - 7]
                + (seq_t)20992 * cache[idx - 8]
                - (seq_t)40064 * cache[idx - 9]
                - (seq_t)9984  * cache[idx - 10]
                + (seq_t)24576 * cache[idx - 11];
        }
#if defined(__cplusplus)
        cache[i - 1] = val;
#else
        cache[i - 1] = val;
#endif
    }

    cache_computed = (size_t)n;
    return cache[n - 1];
}

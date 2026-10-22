/*
 * A384532  Consecutive states of the linear congruential pseudo-random number
 *          generator 663608941*s mod 2^32 when started at s=1.
 * https://oeis.org/A384532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 663608941 * a(n‑1) mod 2^32; a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_maxn = 0;

    if (n == 0) return 0;   /* not used, but keeps compiler happy */

    /* initialise the cache on first call */
    if (cache == NULL) {
        cache_cap = 2;                /* indices 0..1, 0 unused */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)0;          /* unused */
        cache[1] = (seq_t)1;          /* a(1) */
        cache_maxn = 1;
    }

    if (n <= cache_maxn) return cache[n];

    /* grow the cache if necessary */
    size_t required = (size_t)n + 1;
    if (required > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (required > new_cap) new_cap = required;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* iterate from the first missing index up to n */
    const uint64_t mult = 663608941ULL;
    uint32_t prev = (uint32_t)cache[cache_maxn];
    for (idx_t i = cache_maxn + 1; i <= n; ++i) {
        uint32_t cur = (uint32_t)((uint64_t)prev * mult & 0xFFFFFFFFu);
        cache[i] = (seq_t)cur;
        prev = cur;
    }
    cache_maxn = n;
    return cache[n];
}

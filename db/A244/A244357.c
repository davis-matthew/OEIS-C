/*
 * A244357  Numbers n such that n, n+1, and n+2 are not divisible by any of
 *          their nonzero digits.
 * https://oeis.org/A244357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 427, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A244357: Numbers n such that n, n+1, n+2 are not divisible
   by any of their nonzero digits.  The function computes
   terms on demand and caches them for subsequent calls. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

static bool digit_ok(seq_t x)
{
    seq_t t = x;
    while (t > 0)
    {
        int d = (int)(t % 10);
        if (d != 0 && x % d == 0)
            return false;
        t /= 10;
    }
    return true;
}

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> cache;
static inline void ensure_cache(idx_t n)
{
    if (cache.size() >= n)
        return;
    seq_t cand = cache.empty() ? 1 : cache.back() + 1;
    while (cache.size() < n)
    {
        if (digit_ok(cand) && digit_ok(cand + 1) && digit_ok(cand + 2))
            cache.push_back(cand);
        cand++;
    }
}
#else
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static inline void ensure_cache(idx_t n)
{
    if (cache_len >= (size_t)n)
        return;
    size_t need = n;
    if (need > cache_cap)
    {
        size_t newcap = cache_cap ? cache_cap * 2 : 8;
        while (newcap < need)
            newcap *= 2;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache_cap = newcap;
    }
    seq_t cand = cache_len ? cache[cache_len - 1] + 1 : 1;
    while (cache_len < (size_t)n)
    {
        if (digit_ok(cand) && digit_ok(cand + 1) && digit_ok(cand + 2))
            cache[cache_len++] = cand;
        cand++;
    }
}
#endif

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined, but safe */
    ensure_cache(n);
#ifdef __cplusplus
    return cache[n - 1];
#else
    return cache[n - 1];
#endif
}

/*
 * A356061  Numbers whose sum of digits is a refactorable number.
 * https://oeis.org/A356061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose digit sum is refactorable (s % τ(s) == 0). */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"
#ifdef __cplusplus
#include <vector>
#endif

/* keep the last tested candidate value */
static uint64_t next_candidate = 0;

/* small divisor count for a 64‑bit integer (used for the sum of digits) */
static int tau_uint64(uint64_t n)
{
    int res = 1;
    for (uint64_t p = 2; p * p <= n; ++p)
    {
        if (n % p == 0)
        {
            int e = 0;
            while (n % p == 0)
            {
                ++e;
                n /= p;
            }
            res *= (e + 1);
        }
    }
    if (n > 1)
        res *= 2;
    return res;
}

#ifdef __cplusplus
/* C++ build: use std::vector for the cache */
static std::vector<seq_t> cache;
#else
/* C build: manual dynamic array */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap == 0 ? 128 : cache_cap * 2;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = tmp;
    cache_cap = new_cap;
}
#endif

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for offset 0 */

#ifdef __cplusplus
    /* C++: use vector */
    while (cache.size() < n)
    {
        ++next_candidate;
        seq_t cand = (seq_t)next_candidate;
        seq_t sum = digit_sum(cand);
        uint64_t s = (uint64_t)sum;          /* sum fits into 64 bits */
        int dcnt = tau_uint64(s);
        if (s % (uint64_t)dcnt == 0)
            cache.push_back(cand);
    }
    return cache[n - 1];

#else
    /* C: use dynamic array */
    while (cache_len < n)
    {
        ++next_candidate;
        seq_t cand = (seq_t)next_candidate;
        seq_t sum = digit_sum(cand);
        uint64_t s = (uint64_t)sum;
        int dcnt = tau_uint64(s);
        if (s % (uint64_t)dcnt == 0)
        {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = cand;
        }
    }
    return cache[n - 1];
#endif
}

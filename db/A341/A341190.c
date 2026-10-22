/*
 * A341190  Numbers that when divided by the sum of their digits leave 20 as
 *          remainder.
 * https://oeis.org/A341190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 771, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the increasing sequence of numbers x such that
   x % digit_sum(x) == 20. The implementation builds the sequence on
   demand, caching the found terms. */
#include <stdlib.h>
#include <stddef.h>
#include "../utils/digit_sum.h"

/* Static cache for the sequence values */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t last_candidate = 0;  /* last tested candidate */

/* Ensure the cache has capacity for at least 'needed' elements */
static void ensure_cache(size_t needed)
{
    if (cache_cap < needed)
    {
        size_t new_cap = cache_cap ? cache_cap << 1 : 8;
        while (new_cap < needed) new_cap <<= 1;
        /* Cast to seq_t* to satisfy C++ compilers */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) /* sequence is defined for n >= 1, but guard against 0 */
        return (seq_t)0;

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n)
    {
        seq_t cand = last_candidate + (seq_t)1;
        for (;;)
        {
            seq_t s = digit_sum(cand);
            if (s && (cand % s == (seq_t)20))
                break;
            cand = cand + (seq_t)1;
        }
        last_candidate = cand;
        ensure_cache(cache_len + 1);
        cache[cache_len++] = cand;
    }

    return cache[(size_t)n - 1];
}

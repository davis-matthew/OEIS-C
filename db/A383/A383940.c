/*
 * A383940  Consecutive states of the linear congruential pseudo-random number
 *          generator (25173*s+13849) mod 2^16 when started at s=1.
 * https://oeis.org/A383940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; a(n) = (25173*a(n-1)+13849) mod 2^16 for n > 1
 * Range:     u8 n <= 1, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *seq_cache = NULL;
static size_t cache_capacity = 0;
static idx_t cache_size = 0; /* maximum n that has been computed */

/* Grow the cache so that it can hold at least new_cap elements indexed from 0. */
static void ensure_capacity(size_t new_cap)
{
    if (cache_capacity >= new_cap)
        return;

    size_t cap = cache_capacity ? cache_capacity * 2 : 16;
    while (cap < new_cap)
        cap *= 2;

    seq_t *tmp = (seq_t *)realloc(seq_cache, cap * sizeof(seq_t));
    if (!tmp)
        abort(); /* memory allocation failed */

    seq_cache = tmp;
    cache_capacity = cap;
}

/* Compute the n-th term of the LCG sequence. */
seq_t a(idx_t n)
{
    if (n == 0) /* sequence offset starts at 1, undefined for 0 */
        return (seq_t)0; /* arbitrary, never used */

    /* Ensure the cache holds at least index 1. */
    if (cache_size == 0) {
        /* Allocate for at least index 1. */
        ensure_capacity(1 + 1); /* 0 unused, index 1 used */
        seq_cache[1] = (seq_t)1; /* a(1) = 1 */
        cache_size = 1;
    }

    /* If the requested n is beyond what we've cached, extend the cache. */
    if ((idx_t)cache_size < n) {
        ensure_capacity((size_t)n + 1); /* +1 for 0 unused slot */
        for (size_t i = cache_size + 1; i <= (size_t)n; ++i) {
            /* Perform the recurrence using a 32‑bit intermediate to avoid overflow. */
            uint32_t temp = 25173u * (uint32_t)seq_cache[i - 1] + 13849u;
            seq_cache[i] = (seq_t)(temp & 0xFFFFu); /* mod 2^16 */
        }
        cache_size = n;
    }

    return seq_cache[n];
}

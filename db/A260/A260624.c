/*
 * A260624  a(n) = arithmetic derivative of the n-th composite number.
 * https://oeis.org/A260624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = arithmetic derivative of the n-th composite number
 * Range:     u8 n <= 70, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   derivative_uint64, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_realloc.h"
#include "../utils/derivative_uint64.h"

/* Simple primality test for 64‑bit unsigned integers */
static bool is_prime_uint64(uint64_t x)
{
    if (x < 2)            return false;
    if (x == 2 || x == 3) return true;
    if ((x & 1) == 0)     return false;
    for (uint64_t i = 3; i * i <= x; i += 2)
    {
        if (x % i == 0) return false;
    }
    return true;
}

/* Cache of previously computed values */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 4;  /* next integer to test for compositeness */

/* Ensure that the cache contains at least n entries; return the n-th value */
static seq_t ensure_and_get(idx_t n)
{
    if (n <= cache_len) return cache[n - 1];

    while (cache_len < n)
    {
        /* Find the next composite number */
        while (true)
        {
            if (next_candidate >= 4 && !is_prime_uint64(next_candidate))
                break;
            ++next_candidate;
        }

        /* Compute its arithmetic derivative */
        seq_t val = derivative_uint64(next_candidate);

        /* Grow cache if necessary */
        if (cache_len == cache_cap)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            seq_t *new_arr = seq_realloc(cache, cache_cap, new_cap);
            cache = new_arr;
            cache_cap = new_cap;
        }

        cache[cache_len++] = val;
        ++next_candidate;
    }
    return cache[n - 1];
}

seq_t a(idx_t n)
{
    return ensure_and_get(n);
}

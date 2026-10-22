/*
 * A386115  Primes having only {1, 3, 6, 8} as digits.
 * https://oeis.org/A386115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 140, u32 n <= 49, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th prime number whose decimal digits belong only to
   the set {1,3,6,8}.  The numbers are generated in increasing order
   by iterating over the 4‑ary representations of successive lengths
   and testing each candidate for primality. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#ifdef __cplusplus
#include <vector>
#endif

static const uint8_t DIG_MAP[4] = {1, 3, 6, 8};

/* Return the next integer composed solely of the digits 1,3,6,8
   in increasing numeric order. */
static seq_t next_allowed_number(void)
{
    static idx_t len = 1;          /* current number of decimal digits   */
    static idx_t pos = 0;          /* index among the 4^len candidates */

    seq_t n = (seq_t)0;
    idx_t tmp = pos;

    /* Build the number from most significant to least significant
       digit by interpreting `tmp` as a base‑4 number. */
    for (idx_t i = 0; i < len; ++i)
    {
        idx_t shift = 2 * (len - 1 - i);     /* 2 bits per digit  */
        uint8_t d = DIG_MAP[(tmp >> shift) & 3];
        n = n * 10 + (seq_t)d;
    }

    /* Advance to the next candidate. */
    ++pos;
    idx_t maxpos = (idx_t)1 << (2 * len); /* 4^len */
    if (pos == maxpos)
    {
        pos = 0;
        ++len;
    }
    return n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n==0 */

    /* C++ build – use std::vector for automatic growth. */
#ifdef __cplusplus
    static std::vector<seq_t> cache;
    while (cache.size() < n)
    {
        seq_t cand = next_allowed_number();
        if (is_prime_big(cand))
            cache.push_back(cand);
    }
    return cache[n-1];

#else   /* C build – use manual dynamic array. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    while (cache_len < n)
    {
        seq_t cand = next_allowed_number();
        if (is_prime_big(cand))
        {
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) return (seq_t)0;   /* allocation failure */
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
    }
    return cache[n-1];
#endif
}

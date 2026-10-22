/*
 * A382896  Smith sphenic numbers, i.e., Smith numbers (A006753) that are the
 *          product of three distinct prime numbers.
 * https://oeis.org/A382896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 317, u32 n <= 7523, u64 n <= 7523,
 *            u128 n <= 7523, big any n
 * Helpers:   digit_sum, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: find Smith sphenic numbers (Smith numbers that are product of
   three distinct primes).  The returned sequence a(n) is the n‑th such
   number, with a(1) = 438. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/digit_sum.h"

/* Find the next Smith sphenic number greater than any found so far. */
static uint64_t _find_next_smith_sphenic(void)
{
    static uint64_t candidate = 437;           /* start before 438 */
    uint64_t primes[16], exps[16];
    size_t cnt;

    while (1) {
        ++candidate;
        factor_u64(candidate, primes, exps, &cnt);

        /* Must be exactly three distinct prime factors. */
        if (cnt != 3) continue;
        if (exps[0] != 1 || exps[1] != 1 || exps[2] != 1) continue;

        seq_t sum_num     = digit_sum((seq_t)candidate);
        seq_t sum_factors = 0;
        for (size_t i = 0; i < cnt; ++i)
            sum_factors += digit_sum((seq_t)primes[i]);

        if (sum_num == sum_factors)
            return candidate;                  /* Smith sphenic number found */
    }
}

/* Main sequence function.  It memoises the terms in a static cache so
   calls may arrive in any order. */
seq_t a(idx_t n)
{
    static seq_t *cache        = NULL;
    static size_t cache_size   = 0;
    static size_t cache_cap    = 0;

    /* Grow the cache until it contains at least n terms. */
    while (cache_size < (size_t)n) {
        if (cache_cap == 0) {
            cache_cap = 256;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        } else if (cache_size == cache_cap) {
            cache_cap <<= 1;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_size++] = (seq_t)_find_next_smith_sphenic();
    }

    return cache[(size_t)n - 1];
}

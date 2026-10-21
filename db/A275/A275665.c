/*
 * A275665  Numbers n such that n and sopf(n) are relatively prime, where
 *          sopf(n) (A008472) is the sum of the distinct primes dividing n.
 * https://oeis.org/A275665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 144, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A275665: numbers n such that n and sopf(n) are relatively prime.
 * Computed by incremental search: test each integer until the nth term
 * is found, using gcd with the sum of distinct prime factors.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

static bool is_good_uint64(uint64_t val)
{
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64(val, pr, ex, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i)
        sum += (seq_t)pr[i];

    seq_t g = gcd((seq_t)val, sum);
    return g == 1;
}

seq_t a(idx_t n)
{
    /* n is 1‑based.  The sequence is defined for n >= 1. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_candidate = 1;

    if (n == 0)
        return 0;           /* not used, but keeps type consistent */

    if (cache_len >= (size_t)n)
        return cache[n - 1];

    while (cache_len < (size_t)n) {
        if (is_good_uint64((uint64_t)next_candidate)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    exit(EXIT_FAILURE);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}

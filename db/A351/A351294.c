/*
 * A351294  Numbers whose multiset of prime factors has at least one
 *          permutation with all distinct run-lengths.
 * https://oeis.org/A351294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A351294: Numbers whose prime exponents are all distinct.
   This function returns the n-th such number, where a(1)=1. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Test whether all prime exponents of x are distinct. */
static bool exponents_distinct(uint64_t x)
{
    if (x <= 1U)            /* 1 has no prime factors – qualifies */
        return true;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64(x, primes, exps, &cnt);

    /* Exponents are few (≤15 for 64‑bit numbers), so a quadratic test is fine. */
    for (size_t i = 0; i < cnt; ++i)
        for (size_t j = i + 1; j < cnt; ++j)
            if (exps[i] == exps[j])
                return false;

    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;        /* cache of computed terms */
    static size_t cache_cap = 0;       /* capacity of the cache */
    static size_t cache_len = 0;       /* number of terms stored */
    static uint64_t next_candidate = 1;/* next integer to test */

    if (n == 0) /* out of range, but not expected */
        return (seq_t)0;

    /* extend cache until we have at least n terms */
    while (cache_len < n) {
        if (exponents_distinct(next_candidate)) {
            /* grow cache if necessary */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache_cap = new_cap;
            }
            cache[cache_len] = (seq_t)next_candidate;
            ++cache_len;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}

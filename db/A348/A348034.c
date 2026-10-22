/*
 * A348034  Numbers k that have at least one unitary divisor d such that
 *          sigma(d)*d is equal to k.
 * https://oeis.org/A348034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search‑based computation of A348034: a(n) is the nth integer k that
   has a unitary divisor d with sigma(d)*d == k.  The function iterates
   through successive integers and tests the condition, caching all
   found terms.  It stops once the required n‑th term is found.  The
   implementation uses the provided factor_u64 helper for prime
   factorisation. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Test whether n has a unitary divisor d such that sigma(d)*d == n. */
static bool has_property(uint64_t n)
{
    /* Temporary storage for prime factors and exponents. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64(n, primes, exps, &cnt);

    /* Pre–compute p^e and sigma(p^e) for each prime factor. */
    seq_t p_pow[64];
    seq_t sigma_p[64];
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t pe = 1;
        seq_t sigma = 1;
        for (size_t j = 0; j < exps[i]; ++j) {
            pe *= p;
            sigma = sigma * p + 1;
        }
        p_pow[i] = pe;
        sigma_p[i] = sigma;
    }

    /* Enumerate all unitary divisors: subsets of the prime power list. */
    size_t total = (size_t)1 << cnt;
    for (size_t mask = 0; mask < total; ++mask) {
        seq_t d = 1;
        seq_t sig = 1;
        for (size_t i = 0; i < cnt; ++i) {
            if (mask & ((size_t)1 << i)) {
                d *= p_pow[i];
                sig *= sigma_p[i];
            }
        }
        if (sig * d == (seq_t)n) {
            return true;
        }
    }
    return false;
}

/* Cache for the discovered sequence values. */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms stored */
static size_t cache_cap = 0;   /* allocated capacity */
static uint64_t next_candidate = 1;

/* Ensure the cache has enough capacity for at least 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Required function. */
seq_t a(idx_t n)
{
    /* Compute sequence values until we have at least n terms. */
    while (cache_len < (size_t)n) {
        if (has_property(next_candidate)) {
            ensure_capacity(cache_len + 1);
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}

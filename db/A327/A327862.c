/*
 * A327862  Numbers whose arithmetic derivative is of the form 4k+2, cf.
 *          A003415.
 * https://oeis.org/A327862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 8134, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the nth odd integer whose arithmetic derivative ≡ 2 (mod 4).
   For a number n = ∏pᵢ^eᵢ, its arithmetic derivative is
   n' = Σ(eᵢ · n / pᵢ).  The function tests successive integers,
   checks this condition modulo 4, caches the terms, and returns the nth term. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static size_t cache_cap = 0;

/* Return true if the arithmetic derivative of val is ≡ 2 (mod 4). */
static bool deriv_mod4_eq_2(uint64_t val)
{
    size_t cnt;
    uint64_t primes[64], exps[64];
    factor_u64(val, primes, exps, &cnt);

    unsigned int sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        unsigned int a   = (unsigned int)(exps[i] & 3ULL);          /* eᵢ mod 4 */
        unsigned int nd  = (unsigned int)((val / primes[i]) & 3ULL); /* (n/pᵢ) mod 4 */
        unsigned int term = (a * nd) & 3U;                          /* (a·nd) mod 4 */
        sum = (sum + term) & 3U;
    }
    return sum == 2U;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined by the spec */

    if (n <= cache_len) return cache[n - 1];

    seq_t next = (cache_len == 0) ? (seq_t)1 : cache[cache_len - 1] + (seq_t)1;

    while (cache_len < n) {
        /* Only factor values that fit into 64 bits. */
        if (next > (seq_t)UINT64_MAX) abort();
        uint64_t cand = (uint64_t)next;

        if (cand > 1 && deriv_mod4_eq_2(cand)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next;
        }
        ++next;
    }
    return cache[n - 1];
}

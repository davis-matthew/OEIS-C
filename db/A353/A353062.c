/*
 * A353062  Non-prime-powers k such that sigma(k^2) is divisible by d(k^2),
 *          where d = A000005, sigma = A000203; non-prime-powers k such that
 *          k^2 is in A003601.
 * https://oeis.org/A353062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 253, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_to_cache, big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A353062
   Formula: a(n) is the nth positive integer k (k ≥ 1) that
            • k is not a prime power,
            • σ(k²) is divisible by d(k²)  (σ = sum of divisors, d = divisor count),
            • and k² belongs to A003601 (numbers where σ(k²)ᵤ / d(k²)ᵤ is an integer). */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
/* Helpers from the utilities repository */
#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/big_pow.h"      /* seq_t big_pow(seq_t base, seq_t exp); */
#include "../utils/append_to_cache.h"

/* test whether k is a non‑prime‑power and whether σ(k²) % d(k²) == 0 */
static bool good_k(uint64_t k)
{
    if (k == 1ULL)   /* 1 is treated as a valid term */
        return true;

    uint64_t primes[32], exps[32];
    size_t cnt = 0;
    factor_u64(k, primes, exps, &cnt);
    if (cnt == 1)   /* prime power; exclude */
        return false;

    seq_t d_m = 1;      /* divisor count of k² = ∏ (2e_i + 1) */
    seq_t sigma_m = 1;  /* σ(k²)  */

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        uint64_t e  = exps[i];           /* exponent of p in k */
        uint64_t e2 = e * 2;             /* exponent of p in k² */

        seq_t numerator   = big_pow(p, (seq_t)(e2 + 1)) - 1;
        seq_t denominator = p - 1;
        seq_t part = numerator / denominator;   /* exact division of integers */

        sigma_m = sigma_m * part;
        d_m = d_m * (seq_t)(e2 + 1);
    }

    return (sigma_m % d_m) == 0;
}

/* static cache of computed terms */
static seq_t *cache   = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static uint64_t last_checked = 1;   /* next k to test */

seq_t a(idx_t n)
{
    /* grow cache until it contains at least n terms */
    while (cache_len < (size_t)n) {
        if (good_k(last_checked))
            append_to_cache(&cache, &cache_cap, &cache_len, (seq_t)last_checked);
        ++last_checked;
    }
    return cache[n - 1];
}

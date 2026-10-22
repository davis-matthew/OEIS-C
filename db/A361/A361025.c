/*
 * A361025  a(n) = A007814(sigma(n)) - A007814(n), where A007814(n) gives the
 *          2-adic valuation of n, and sigma is the sum of divisors function.
 * https://oeis.org/A361025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = v₂(σ(n)) – v₂(n) where σ(n) is the sum of divisors of n
 * Range:     i8 n <= 48, i16 n <= 9772, i32 n <= 9772, i64 n <= 9772,
 *            i128 n <= 9772, big any n
 * Helpers:   build_primes_upto, floor_sqrt, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"   // build_primes_upto(limit, uint64_t **out, size_t *cnt)
#include "../utils/floor_sqrt.h"         // floor_sqrt(x) → seq_t
#include "../utils/pow_seq.h"            // pow_seq(base, exp) → seq_t

static uint64_t *primes = NULL;     /* cache of primes */
static size_t     prime_cnt = 0;
static idx_t      prime_limit = 0;

/* Ensure we have all primes up to `limit`.  Rebuilds the cache if needed. */
static void
ensure_primes(idx_t limit) {
    if (limit <= prime_limit) return;
    /* Free previous cache if any – harmless for a single run. */
    if (primes) free(primes);
    build_primes_upto(limit, &primes, &prime_cnt);
    prime_limit = limit;
}

static int
v2_seq(seq_t x) {
    int cnt = 0;
    while ((x & 1) == 0 && x != 0) {
        x >>= 1;
        ++cnt;
    }
    return cnt;
}

seq_t a(idx_t n) {
    /* 1. 2‑adic valuation of n */
    seq_t v2n = 0;
    seq_t tmp = (seq_t)n;
    while ((tmp & 1) == 0 && tmp != 0) {
        tmp >>= 1;
        ++v2n;
    }

    /* 2. Factorise n and compute σ(n) */
    idx_t sqrt_n = (idx_t)floor_sqrt((seq_t)n);
    ensure_primes(sqrt_n);

    seq_t sigma = 1;
    uint64_t m = (uint64_t)n;

    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= m; ++i) {
        uint64_t p = primes[i];
        if (m % p != 0) continue;
        int e = 0;
        while (m % p == 0) { m /= p; ++e; }

        seq_t numerator   = pow_seq((seq_t)p, e + 1) - 1;
        seq_t denominator = (seq_t)p - 1;
        seq_t factor      = numerator / denominator;      /* exact division */
        sigma *= factor;
    }
    if (m > 1) {                      /* remaining prime factor */
        sigma *= ((seq_t)m + 1);       /* (p²–1)/(p–1) = p+1 for e=1 */
    }

    /* 3. 2‑adic valuation of σ(n) */
    int v2sigma = v2_seq(sigma);

    return (seq_t)v2sigma - v2n;
}

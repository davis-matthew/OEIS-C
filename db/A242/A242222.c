/*
 * A242222  Number of primes p <= (prime(n)+1)/2 such that the harmonic number
 *          H(p-1) = sum_{0<k<p} 1/k is a primitive root modulo prime(n), or 0
 *          if such a prime p does not exist.
 * https://oeis.org/A242222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ prime p ≤ (prime(n)+1)/2 : H(p-1) is a primitive root
 *            modulo prime(n) } H(p-1) = Σ_{k=1}^{p-1} 1/k (mod prime(n)) The
 *            function uses prime sieving, modular exponentiation, and
 *            factorisation of prime(n)-1
 * Range:     u8 n <= 36, u16 n <= 700, u32 n <= 700, u64 n <= 700,
 *            u128 n <= 700, big any n
 * Helpers:   build_primes_upto, build_spf, get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utils/build_primes_upto.h"
#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"
#include "../utils/build_spf.h"

/* Global caches */
static uint64_t *prime_cache   = NULL;   /* primes ≤ cached_limit */
static size_t    prime_cnt     = 0;
static idx_t     prime_cached_limit = 0;

static idx_t *spf_cache      = NULL;   /* smallest prime factor array */
static idx_t   spf_cached_limit = 0;

/* Ensure that prime_cache contains all primes ≤ limit */
static void ensure_primes_up_to(idx_t limit)
{
    if (prime_cache == NULL || limit > prime_cached_limit) {
        if (prime_cache != NULL) free(prime_cache);
        build_primes_upto(limit, &prime_cache, &prime_cnt);
        prime_cached_limit = limit;
    }
}

/* Ensure that spf_cache contains SPF for all numbers ≤ limit */
static void ensure_spf_up_to(idx_t limit)
{
    if (spf_cache == NULL || limit > spf_cached_limit) {
        if (spf_cache != NULL) free(spf_cache);
        build_spf(limit, &spf_cache);
        spf_cached_limit = limit;
    }
}

/* Count distinct prime factors of n using spf_cache (n ≥ 2) */
static size_t get_distinct_prime_factors(idx_t n, uint32_t *out)
{
    size_t cnt = 0;
    while (n > 1) {
        idx_t p = spf_cache[n];
        out[cnt++] = (uint32_t)p;
        while (n % p == 0) n /= p;
    }
    return cnt;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* safety for zero index */

    /* Prime(n) */
    idx_t prime_n = get_prime(n);

    /* Upper bound for candidate primes */
    idx_t limit_p = (prime_n + 1) / 2;

    /* Ensure needed data is available */
    ensure_primes_up_to(limit_p);
    ensure_spf_up_to(prime_n - 1);

    /* Compute prefix sums of inverses modulo prime_n up to limit_p-1 */
    idx_t max_k = limit_p - 1;                 /* maximum k needed */
    seq_t *hprefix = (seq_t *)malloc((max_k + 1) * sizeof(seq_t));
    seq_t sum = 0;
    seq_t mod = (seq_t)prime_n;

    for (idx_t i = 1; i <= max_k; ++i) {
        seq_t inv = mod_pow((seq_t)i, prime_n - 2, mod);
        sum += inv;
        sum %= mod;
        hprefix[i] = sum;
    }

    /* Distinct prime factors of prime_n-1 */
    uint32_t factors[64];
    size_t nfactors = get_distinct_prime_factors(prime_n - 1, factors);

    /* Count qualifying primes */
    seq_t result = 0;
    for (size_t i = 0; i < prime_cnt && prime_cache[i] <= limit_p; ++i) {
        idx_t candidate = prime_cache[i];
        /* For candidate 2, H(1)=1 never primitive. */
        if (candidate < 3) continue;

        seq_t val = hprefix[candidate - 1];
        if (val == 0 || val == 1) continue;   /* cannot be primitive root */

        bool ok = true;
        for (size_t f = 0; f < nfactors; ++f) {
            idx_t q = factors[f];
            seq_t power = mod_pow(val, (prime_n - 1) / q, mod);
            if (power == 1) { ok = false; break; }
        }
        if (ok) ++result;
    }

    free(hprefix);
    return result;
}

/*
 * A324526  Numbers m such that gcd(sigma(m), pod(m)) = tau(m) where tau(k) =
 *          the number of divisors of k (A000005), sigma(k) = the sum of the
 *          divisors of k (A000203) and pod(n) = the product of divisors of k
 *          (A007955).
 * https://oeis.org/A324526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   build_primes_upto, floor_sqrt, gcd, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers m such that gcd(sigma(m), pod(m)) = tau(m)
   Computation: brute‑force search, checking each integer and storing
   valid ones. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/gcd.h"
#include "../utils/mod_pow.h"
#include "../utils/floor_sqrt.h"
#include "../utils/build_primes_upto.h"

static uint64_t *primes = NULL;
static size_t primes_cnt = 0;
static idx_t prime_limit = 0;

static void ensure_primes(idx_t m)
{
    /* primes must cover sqrt(m) */
    seq_t s = floor_sqrt((seq_t)m);
    idx_t need = (idx_t)s + 1;
    if (need > prime_limit) {
        if (primes) free(primes);
        build_primes_upto(need, &primes, &primes_cnt);
        prime_limit = need;
    }
}

static bool passes(uint64_t m)
{
    idx_t temp = m;
    idx_t tau   = 1;   /* number of divisors */
    idx_t sigma = 1;   /* sum of divisors (exact integer) */

    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        uint64_t p = primes[i];
        if (temp % p == 0) {
            int exp = 0;
            idx_t p_pow = 1;  /* p^exp */
            while (temp % p == 0) {
                temp /= p;
                ++exp;
                p_pow *= p;
            }
            /* sum of divisors for this prime power: (p^(exp+1)-1)/(p-1) */
            idx_t part_sigma = (idx_t)((( (__uint128_t)p_pow * p - 1ULL) / (p - 1ULL)));
            sigma *= part_sigma;
            tau   *= (idx_t)(exp + 1);
        }
    }
    if (temp > 1) {          /* remaining prime factor */
        tau   *= 2;
        sigma *= (idx_t)(1 + temp);
    }

    /* pod(m) = m^(tau/2)  (when tau is even) */
    /*          = m^((tau-1)/2) * sqrt(m)  (when tau is odd) */
    idx_t sigma_idx = sigma;
    seq_t sigma_t   = (seq_t)sigma_idx;
    seq_t base      = (seq_t)(m % sigma_idx);
    seq_t pod_mod;

    if (tau % 2 == 0) {
        pod_mod = mod_pow(base, tau / 2, sigma_t);
    } else {
        seq_t sqrt_m   = floor_sqrt((seq_t)m);
        seq_t part     = mod_pow(base, (tau - 1) / 2, sigma_t);
        seq_t sqrt_mod = sqrt_m % sigma_t;
        pod_mod = (seq_t)(((__uint128_t)part * sqrt_mod) % sigma_t);
    }

    seq_t g = gcd(sigma_t, pod_mod);
    return g == (seq_t)tau;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* Undefined, but safe */

    /* static state across calls */
    static seq_t *seq          = NULL;
    static size_t seq_len      = 0;
    static size_t seq_cap      = 0;
    static uint64_t current_m  = 1;

    /* Cache primes large enough for current range */
    if (!primes) ensure_primes(current_m);

    while (seq_len < (size_t)n) {
        if (passes(current_m)) {
            if (seq_len >= seq_cap) {
                seq_cap = (seq_cap == 0) ? 16 : seq_cap * 2;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = (seq_t)current_m;
        }
        ++current_m;
        /* Expand primes if necessary for the new current_m */
        ensure_primes(current_m);
    }

    return seq[n - 1];
}

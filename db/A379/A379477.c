/*
 * A379477  Numbers k such that A003961(k)-2k and A003961(k)-sigma(k) have a
 *          common divisor > 1, where A003961 is fully multiplicative with
 *          a(prime(i)) = prime(i+1), and sigma is the sum of divisors
 *          function.
 * https://oeis.org/A379477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1939, u32 n <= 9510, u64 n <= 9510,
 *            u128 n <= 9510, big any n
 * Helpers:   append_candidate, gcd, isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes numbers k such that gcd(A003961(k)-2k, A003961(k)-sigma(k)) > 1
   by generating candidate k in increasing order, factoring it with a
   linear sieve, computing A003961(k) and sigma(k), and testing the gcd. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/append_candidate.h"
#include "../utils/isPrimeTrial.h"

#define LIMIT 5000000     /* upper bound for k to generate many terms */

/* -------------------------------------------------------------------------- */
/*  Global state for sieve                                       */
/* -------------------------------------------------------------------------- */

static uint32_t *primes = NULL;      /* list of primes up to LIMIT           */
static size_t    prime_cnt = 0;      /* number of primes                     */
static uint32_t *spf = NULL;        /* smallest prime factor table          */

/* -------------------------------------------------------------------------- */
/*  Linear sieve to build prime list and spf table                          */
/* -------------------------------------------------------------------------- */

static void init_sieve(void)
{
    size_t pr_alloc = LIMIT / 5 + 200;            /* enough room for primes */
    primes = (uint32_t *)malloc(pr_alloc * sizeof(uint32_t));
    if (!primes) abort();

    spf = (uint32_t *)calloc((size_t)LIMIT + 1, sizeof(uint32_t));
    if (!spf) abort();

    prime_cnt = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            primes[prime_cnt++] = i;
        }
        for (size_t j = 0; j < prime_cnt && (uint64_t)i * primes[j] <= LIMIT; ++j) {
            uint32_t v = i * primes[j];
            spf[v] = primes[j];
            if (i % primes[j] == 0)
                break;
        }
    }
}

/* -------------------------------------------------------------------------- */
/*  Return the prime immediately following the given prime p                */
/* -------------------------------------------------------------------------- */

static uint32_t next_prime_of(uint32_t p)
{
    /* binary search for p in primes[] */
    size_t l = 0, r = prime_cnt;
    while (l < r) {
        size_t m = (l + r) >> 1;
        if (primes[m] >= p)
            r = m;
        else
            l = m + 1;
    }
    if (l < prime_cnt && primes[l] == p) {
        if (l + 1 < prime_cnt)
            return primes[l + 1];
        /* p is the largest precomputed prime; find next by trial */
        uint32_t cand = primes[l] + 1;
        while (!isPrimeTrial((seq_t)cand))
            ++cand;
        return cand;
    }
    /* Should never happen for a valid prime input */
    return 0;
}

/* -------------------------------------------------------------------------- */
/*  Factorize k using spf and compute A003961(k) as well as sigma(k)        */
/* -------------------------------------------------------------------------- */

static void compute_factors(uint32_t k, seq_t *out_a003961, seq_t *out_sigma)
{
    seq_t a003961 = 1;
    seq_t sigma   = 1;
    uint32_t x = k;

    while (x > 1) {
        uint32_t p = spf[x];
        int exp = 0;
        while (x % p == 0) {
            x /= p;
            ++exp;
        }

        /* sigma part: 1 + p + ... + p^exp */
        seq_t sigma_p = 1;
        seq_t pow = 1;
        for (int i = 0; i < exp; ++i) {
            pow *= (seq_t)p;
            sigma_p += pow;
        }
        sigma *= sigma_p;

        /* A003961 part: (next prime after p)^exp */
        uint32_t np = next_prime_of(p);
        seq_t base = (seq_t)np;
        seq_t pow_a = 1;
        for (int i = 0; i < exp; ++i)
            pow_a *= base;
        a003961 *= pow_a;
    }

    *out_a003961 = a003961;
    *out_sigma   = sigma;
}

/* -------------------------------------------------------------------------- */
/*  Public sequence function                                                */
/* -------------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static uint64_t next_k = 1;          /* next integer to test (starting from 1) */

    if (!initialized) {
        init_sieve();
        initialized = true;
    }

    while (cache_len < n) {
        uint32_t k = (uint32_t)next_k++;
        seq_t a003961, sigma;
        compute_factors(k, &a003961, &sigma);

        seq_t two_k = (seq_t)k * 2;
        seq_t d1 = (a003961 > two_k) ? (a003961 - two_k) : (two_k - a003961);
        seq_t d2 = (a003961 > sigma)   ? (a003961 - sigma)   : (sigma - a003961);

        seq_t g = gcd(d1, d2);
        if (g > 1)
            append_candidate(&cache, &cache_len, &cache_cap, (seq_t)k);
    }

    return cache[n - 1];
}

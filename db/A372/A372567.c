/*
 * A372567  Odd numbers k such that k, sigma(k) and A003961(k) have a common
 *          divisor larger than 1, where A003961(n) is fully multiplicative
 *          function with a(prime(i)) = prime(i+1).
 * https://oeis.org/A372567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 39, u32 n <= 8813, u64 n <= 8813,
 *            u128 n <= 8813, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Function: a(n) returns the nth odd number k such that
   gcd(k, σ(k), A003961(k)) > 1.
   Implemented by sieving up to a fixed limit and scanning odd integers. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"

/* --------- Configuration ----------------------------------- */
#define SIEVE_LIMIT 2000000ULL   /* sufficient for the first 10 000 terms */

/* --------- Global tables ----------------------------------- */
static uint32_t *spf         = NULL; /* smallest prime factor */
static uint32_t *next_prime  = NULL; /* next prime after a given prime */
static uint32_t *prime_list  = NULL; /* list of primes up to SIEVE_LIMIT */
static size_t    prime_count = 0;

/* --------- Sequence cache ---------------------------------- */
static seq_t *cache      = NULL; /* cached sequence terms */
static size_t cache_len   = 0;   /* number of cached terms   */
static size_t cache_cap   = 0;   /* allocated cache capacity */
static idx_t  next_candidate = 3; /* next odd number to test   */

/* --------- Ensure sieve is built -------------------------------- */
static void
init_sieve(void)
{
    if (spf != NULL) return;          /* already built */

    spf        = (uint32_t*)calloc((size_t)(SIEVE_LIMIT + 1), sizeof(uint32_t));
    next_prime = (uint32_t*)calloc((size_t)(SIEVE_LIMIT + 1), sizeof(uint32_t));

    size_t prime_est = (size_t)(SIEVE_LIMIT / 10) + 100;
    prime_list = (uint32_t*)malloc(prime_est * sizeof(uint32_t));
    if (!spf || !next_prime || !prime_list) abort();

    prime_count = 0;
    for (uint32_t i = 2; i <= (uint32_t)SIEVE_LIMIT; ++i) {
        if (spf[i] == 0) {               /* i is prime */
            spf[i] = i;
            if (prime_count == prime_est) { /* grow prime list */
                prime_est <<= 1;
                uint32_t *tmp = (uint32_t*)realloc(prime_list, prime_est * sizeof(uint32_t));
                if (!tmp) abort();
                prime_list = tmp;
            }
            prime_list[prime_count++] = i;
            if ((uint64_t)i * (uint64_t)i <= (uint64_t)SIEVE_LIMIT) {
                uint32_t ii = i * i;
                for (uint32_t j = ii; j <= (uint32_t)SIEVE_LIMIT; j += i) {
                    if (spf[j] == 0) spf[j] = i;
                }
            }
        }
    }

    /* build next_prime table */
    for (size_t i = 0; i + 1 < prime_count; ++i) {
        next_prime[prime_list[i]] = prime_list[i + 1];
    }
}

/* --------- Factorise using spf -------------------------------- */
static void
factorize(idx_t n, uint32_t *factors, int *exps, size_t *len)
{
    *len = 0;
    while (n > 1) {
        uint32_t p = spf[(uint32_t)n];
        int e = 0;
        while (n % p == 0) {
            n /= p;
            ++e;
        }
        factors[(*len)++] = p;
        exps[(*len) - 1] = e;
    }
}

/* --------- Ensure cache capacity -------------------------------- */
static void
ensure_cache_cap(size_t cap)
{
    if (cap <= cache_cap) return;
    size_t new_cap = cap ? cap * 2 : 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* --------- Test candidate -------------------------------- */
static bool
candidate_ok(idx_t k)
{
    uint32_t factors[32];
    int exps[32];
    size_t len;
    factorize(k, factors, exps, &len);

    /* sigma(k) */
    uint64_t sigma_val = 1;
    for (size_t i = 0; i < len; ++i) {
        uint64_t term = 1;
        uint64_t cur  = 1;
        for (int j = 0; j < exps[i]; ++j) {
            cur  *= (uint64_t)factors[i];
            term += cur;
        }
        sigma_val *= term;
    }
    seq_t sigma = (seq_t)sigma_val;

    /* A003961(k) */
    uint64_t a_val = 1;
    for (size_t i = 0; i < len; ++i) {
        uint32_t q = next_prime[factors[i]];
        if (q == 0) continue;                     /* no successor prime */
        for (int j = 0; j < exps[i]; ++j) {
            a_val *= (uint64_t)q;
        }
    }
    seq_t a_k = (seq_t)a_val;

    /* gcd checks */
    seq_t g = gcd((seq_t)k, sigma);
    g = gcd(g, a_k);
    return g > (seq_t)1;
}

/* --------- Fill cache up to n terms -------------------------- */
static void
ensure_n_terms(idx_t n)
{
    if (cache_len >= n) return;
    init_sieve();
    while (cache_len < n) {
        if (candidate_ok(next_candidate)) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = (seq_t)next_candidate;
        }
        next_candidate += 2;          /* only odd numbers */
        if ((uint64_t)next_candidate > SIEVE_LIMIT) abort();
    }
}

/* --------- Sequence entry point ----------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) abort();    /* defined for n >= 1 */
    ensure_n_terms(n);
    return cache[n - 1];
}

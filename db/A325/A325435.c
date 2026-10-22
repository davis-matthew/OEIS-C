/*
 * A325435  Numbers m such that m! / sigma(m) is an integer.
 * https://oeis.org/A325435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   build_primes, build_spf, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A325435 – numbers m such that m! / sigma(m) is an integer
   a(n) is the nth such m (offset 1).  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_spf.h"
#include "../utils/build_primes.h"
#include "../utils/sigma_of.h"

/* A safe upper bound for the search range needed to generate 10 000 terms.
   5 000 000 is comfortably large for this sequence.  */
static const idx_t LIMIT = 5000000;

/* Cached auxiliary data. */
static idx_t *g_spf        = NULL;   /* smallest prime factor table */
static idx_t *g_primes     = NULL;   /* list of primes up to LIMIT   */
static size_t g_prime_cnt  = 0;

/* Test whether a number m satisfies sigma(m) | m!  */
static bool qualifies(idx_t m)
{
    if (m == 1) return true;

    /* Compute sigma(m).  sigma_of returns a seq_t.  */
    seq_t sigma_val = sigma_of(m);

    /* We are going to factor sigma_val.  For numbers up to LIMIT,
       sigma_val comfortably fits into an idx_t.  */
    idx_t sigma = (idx_t)sigma_val;

    /* Check each prime factor of sigma.  */
    while (sigma > 1) {
        idx_t p   = g_spf[sigma];
        idx_t e_sig = 0;
        while (sigma % p == 0) { sigma /= p; ++e_sig; }

        /* Exponent of p in m! is sum_{k=1..∞} floor(m / p^k).  */
        idx_t t   = m;
        idx_t e_fact = 0;
        while (t) { t /= p; e_fact += t; }

        if (e_fact < e_sig) return false;
    }
    return true;
}

/* External function required by the build. */
seq_t a(idx_t n)
{
    /* One-time initialization.  */
    static bool initialized = false;
    if (!initialized) {
        build_spf(LIMIT, &g_spf);
        build_primes(LIMIT, &g_primes, &g_prime_cnt);
        initialized = true;
    }

    /* Cache of sequence values.  */
    static seq_t *cache    = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Next candidate number to test.  */
    static idx_t next_m = 1;

    /* Generate terms until we have at least n of them.  */
    while (cache_len < n) {
        if (qualifies(next_m)) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)next_m;
        }
        ++next_m;
    }

    return cache[n - 1];
}

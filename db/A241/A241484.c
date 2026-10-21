/*
 * A241484  Primes p such that p+2 and p+4 are semiprime.
 * https://oeis.org/A241484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 557, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor, alloc_cache, free_cache, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes OEIS sequence A241484.
   It finds successive prime numbers p and tests whether p+2
   and p+4 are semiprimes (products of exactly two primes,
   allowing equal primes).  The results are memoised in a
   static cache so that calls with arbitrary n are fast. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* Helper functions provided by the build environment */
#include "../utils/is_prime.h"                     /* bool is_prime(idx_t x); */
#include "../utils/_smallest_prime_factor.h"       /* uint64_t _smallest_prime_factor(uint64_t n); */
#include "../utils/alloc_cache.h"                  /* seq_t *alloc_cache(idx_t n); */
#include "../utils/free_cache.h"                   /* void free_cache(seq_t *p); */

/* --------------------------------------------------------------------- */
/* Check whether a 64‑bit integer is a semiprime (product of two primes,
   allowing squares). */
static bool is_semiprime_uint64(uint64_t n)
{
    if (n < 4)                     /* 0,1,2,3 cannot be semiprime */
        return false;

    uint64_t spf = _smallest_prime_factor(n);   /* smallest prime factor */
    if (spf == 0 || spf == n)                    /* n is 0 or prime */
        return false;

    uint64_t other = n / spf;
    return is_prime((idx_t)other);               /* other must be prime */
}

/* --------------------------------------------------------------------- */
/* Static cache of primes that satisfy the condition. */
static seq_t *cache        = NULL;   /* array of primes in the sequence */
static size_t cache_cap    = 0;      /* allocated capacity of the array   */
static size_t cache_len    = 0;      /* number of terms currently cached  */

/* Next integer to test for primality.  Starts at 2. */
static idx_t next_candidate = 2;

/* --------------------------------------------------------------------- */
/* Ensure that the cache has room for at least `needed` terms. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap;
    if (new_cap == 0) new_cap = 8;
    while (new_cap < needed) new_cap *= 2;

    seq_t *new_buf = alloc_cache((idx_t)new_cap);
    if (!new_buf) abort();                /* allocation failure */

    /* Copy existing data to the new buffer. */
    for (size_t i = 0; i < cache_len; ++i)
        new_buf[i] = cache[i];

    /* Free old buffer (appropriate for seq_t type). */
    free_cache(cache);

    cache     = new_buf;
    cache_cap = new_cap;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n < 1; behaviour irrelevant */
        return 0;

    /* Return cached value if we already have it. */
    if ((size_t)n <= cache_len)
        return cache[n - 1];

    /* Extend the cache until we have the n‑th term. */
    while (cache_len < (size_t)n) {
        /* Find the next prime. */
        while (!is_prime(next_candidate))
            ++next_candidate;

        /* Check semiprime conditions for p+2 and p+4. */
        if (is_semiprime_uint64((uint64_t)next_candidate + 2) &&
            is_semiprime_uint64((uint64_t)next_candidate + 4)) {

            /* Store the prime in the cache. */
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len] = (seq_t)next_candidate;
            ++cache_len;
        }

        /* Advance to the next integer to test. */
        ++next_candidate;
    }

    return cache[n - 1];
}

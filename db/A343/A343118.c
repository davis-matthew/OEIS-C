/*
 * A343118  Length of the longest sequence of equidistant primes among the
 *          first n primes.
 * https://oeis.org/A343118
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 128, u16 n <= 128, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the length of the longest arithmetic progression among
    the first n primes (OEIS A343118).  The function assumes that
    n ≥ 2; for n < 2 it returns 0.  It builds a cache of primes up
    to the required limit, uses a bitmap to test primality, and
    performs an O(n²) search for the longest AP.  */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../utils/_prime_limit_for_idx.h"   /* _prime_limit_for_idx(idx_t) */
#include "../utils/build_primes_upto.h"       /* build_primes_upto(idx_t,
                                               uint64_t **, size_t *) */

static uint8_t *prime_exists = NULL;     /* bitmap: 1 if value is prime */
static size_t prime_exists_limit = 0;    /* allocated size of bitmap */
static uint64_t *prime_cache = NULL;     /* all primes up to current limit */
static size_t prime_cache_count = 0;      /* number of primes stored */

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    /* Ensure the cached prime list contains at least n primes. */
    if ((size_t)n > prime_cache_count) {
        /* Find a value of 'limit' that contains at least n primes. */
        idx_t limit = _prime_limit_for_idx(n);

        /* Allocate a larger bitmap if necessary. */
        if ((size_t)limit > prime_exists_limit) {
            uint8_t *tmp = (uint8_t *)calloc((size_t)limit + 1, sizeof(uint8_t));
            if (!tmp) return (seq_t)0;      /* out of memory */
            free(prime_exists);
            prime_exists = tmp;
            prime_exists_limit = (size_t)limit;
        }

        /* Generate all primes up to 'limit'. */
        uint64_t *all_primes = NULL;
        size_t all_cnt = 0;
        build_primes_upto(limit, &all_primes, &all_cnt);

        /* Reset the bitmap and mark the primes. */
        memset(prime_exists, 0, prime_exists_limit * sizeof(uint8_t));
        for (size_t i = 0; i < all_cnt; ++i)
            prime_exists[all_primes[i]] = 1;

        /* Store the primes in the cache. */
        if (all_cnt > prime_cache_count) {
            uint64_t *tmp = (uint64_t *)realloc(prime_cache, all_cnt * sizeof(uint64_t));
            if (!tmp) {
                free(all_primes);
                return (seq_t)0; /* out of memory */
            }
            prime_cache = tmp;
            prime_cache_count = all_cnt;
        }
        memcpy(prime_cache, all_primes, all_cnt * sizeof(uint64_t));
        free(all_primes);
    }

    /* Check all pairs (i, j) with i < j < n for an AP. */
    const uint64_t *p = prime_cache;
    const uint64_t max_prime = p[n - 1];
    size_t best_len = 2; /* at least two primes form an AP */

    for (size_t i = 0; i < (size_t)n; ++i) {
        uint64_t pi = p[i];
        for (size_t j = i + 1; j < (size_t)n; ++j) {
            uint64_t pj = p[j];
            uint64_t d   = pj - pi;          /* common difference */
            size_t len   = 2;                /* pi, pj */

            uint64_t next = pj + d;
            while (next <= max_prime && prime_exists[next]) {
                ++len;
                next += d;
            }

            if (len > best_len)
                best_len = len;

            if (best_len == (size_t)n)   /* cannot be longer than n */
                return (seq_t)best_len;
        }
    }

    return (seq_t)best_len;
}

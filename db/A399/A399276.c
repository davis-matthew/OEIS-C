/*
 * A399276  Number of ways writing the n-th semiprime as a sum of two primes.
 * https://oeis.org/A399276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of Goldbach partitions of the n‑th semiprime
 * Range:     u8 n <= 7914, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* maximum value for which we sieve; enough for n up to ~10000 */
static const idx_t SIEVE_LIMIT = 200000;

/* prime table, smallest‑prime‑factor table, list of primes */
static uint8_t *is_prime = NULL;   /* 0 = composite, 1 = prime */
static idx_t *spf       = NULL;    /* smallest prime factor of i   */
static uint32_t *primes = NULL;    /* primes up to SIEVE_LIMIT      */
static size_t primes_cnt  = 0;

/* storage for the first semiprimes and their Goldbach counts */
static idx_t *semiprimes = NULL;
static size_t semiprimes_cnt = 0;
static size_t semiprimes_cap = 0;

static seq_t *cache = NULL;
static size_t cache_cnt = 0;
static size_t cache_cap = 0;

/* next integer to test for being a semiprime */
static idx_t candidate = 2;

/* whether the global data structure has been initialised */
static bool initialized = false;

/* --------------------------------------------------------------------- */
/* grow the cache array                                                 */
static void grow_cache(void)
{
    if (cache_cnt >= cache_cap) {
        cache_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
    }
}

/* grow the semiprimes array                                         */
static void grow_semiprimes(void)
{
    if (semiprimes_cnt >= semiprimes_cap) {
        semiprimes_cap = semiprimes_cap ? semiprimes_cap * 2 : 32;
        idx_t *tmp = (idx_t *)realloc(semiprimes, semiprimes_cap * sizeof(idx_t));
        if (!tmp) abort();
        semiprimes = tmp;
    }
}

/* sieve of Eratosthenes, fill spf and prime list                     */
static void init_sieve(void)
{
    if (initialized) return;
    initialized = true;

    /* allocate tables */
    is_prime = (uint8_t *)malloc((SIEVE_LIMIT + 1) * sizeof(uint8_t));
    spf      = (idx_t *)malloc((SIEVE_LIMIT + 1) * sizeof(idx_t));
    if (!is_prime || !spf) abort();

    for (idx_t i = 0; i <= SIEVE_LIMIT; ++i) {
        is_prime[i] = 1U;
        spf[i] = i;
    }
    is_prime[0] = is_prime[1] = 0U;

    for (idx_t p = 2; p * p <= SIEVE_LIMIT; ++p) {
        if (is_prime[p]) {
            for (idx_t m = p * p; m <= SIEVE_LIMIT; m += p) {
                is_prime[m] = 0U;
                if (spf[m] == m) spf[m] = p;
            }
        }
    }

    /* first pass: count primes for allocation */
    size_t cnt = 0;
    for (idx_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (is_prime[i]) ++cnt;
    primes_cnt = cnt;
    primes = (uint32_t *)malloc(primes_cnt * sizeof(uint32_t));
    if (!primes) abort();

    /* second pass: store primes */
    size_t pos = 0;
    for (idx_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (is_prime[i]) primes[pos++] = (uint32_t)i;
    }
}

/* --------------------------------------------------------------------- */
/* main sequence function                                              */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for offset 1 */

    init_sieve();

    /* generate semiprimes and cache until we have n of them          */
    while (semiprimes_cnt < n) {

        /* search for next semiprime                                 */
        for (; candidate <= SIEVE_LIMIT; ++candidate) {
            idx_t sp = spf[candidate];
            if (sp < candidate) {                       /* composite */
                idx_t other = candidate / sp;
                if (other >= 2 && spf[other] == other) { /* product of two primes */
                    /* count Goldbach partitions of candidate */
                    idx_t half = candidate >> 1;
                    seq_t cnt = 0;
                    for (size_t i = 0; i < primes_cnt &&
                                      (idx_t)primes[i] <= half; ++i) {
                        idx_t p = (idx_t)primes[i];
                        if (is_prime[candidate - p]) ++cnt;
                    }

                    grow_cache();
                    cache[cache_cnt++] = cnt;

                    grow_semiprimes();
                    semiprimes[semiprimes_cnt++] = candidate;
                    break;               /* go to next semiprime */
                }
            }
        }

        /* advance to next candidate */
        ++candidate;
    }

    return cache[n - 1];
}

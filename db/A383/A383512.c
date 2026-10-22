/*
 * A383512  Heinz numbers of conjugate Wilf partitions.
 * https://oeis.org/A383512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   is_conj_wilf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A383512 by enumerating integers and testing the conjugate Wilf
 * partition property. */
 
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_conj_wilf.h"

/* Largest integer to sieve. Chosen large enough for the first ~10 000 terms. */
#define SIEVE_LIMIT 5000000U

/* Global arrays for the sieve. */
static uint32_t *spf      = NULL; /* smallest prime factor for each number */
static uint32_t *primeIdx = NULL; /* 1‑based index of each prime (0 if composite) */
static uint32_t *primes   = NULL; /* list of primes */
static size_t     primesCnt = 0;
static size_t     primesCap = 0;

/* Cache of the first terms found. */
static seq_t   *cache   = NULL;
static size_t  cacheSize = 0;
static size_t  cacheCap  = 0;

/* State. */
static uint32_t currentX   = 1; /* next integer to test */
static bool initialized = false;

/* Forward declarations. */
static void init_sieve(void);
static bool is_conj_wilf(const uint32_t *parts, size_t len);
static void process_candidate(uint32_t x);

/* Build the sieve with smallest prime factor and prime indices. */
static void init_sieve(void)
{
    uint32_t i, p;
    size_t  j;
    uint32_t limit = SIEVE_LIMIT;

    spf      = (uint32_t *)calloc(limit + 1, sizeof(uint32_t));
    primeIdx = (uint32_t *)calloc(limit + 1, sizeof(uint32_t));

    primesCap = limit / 10; /* crude estimate */
    primes    = (uint32_t *)malloc(sizeof(uint32_t) * primesCap);
    primesCnt = 0;

    for (i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {               /* i is prime */
            spf[i] = i;
            if (primesCnt == primesCap) {
                primesCap <<= 1;
                primes = (uint32_t *)realloc(primes, sizeof(uint32_t) * primesCap);
            }
            primes[primesCnt] = i;
            primeIdx[i] = (uint32_t)(primesCnt + 1); /* 1‑based index */
            ++primesCnt;
        }
        for (j = 0; j < primesCnt && primes[j] <= spf[i] &&
             (uint64_t)i * primes[j] <= limit; ++j) {
            uint32_t prod = i * primes[j];
            spf[prod] = primes[j];
        }
    }
}

/* Process a single candidate integer x: factorize, build the partition
 * of prime indices, test conjugate Wilf property, and add to cache if
 * it passes. */
static void process_candidate(uint32_t x)
{
    uint32_t parts[64];
    size_t   len = 0;
    uint32_t temp = x, p, e;

    while (temp > 1) {
        p = spf[temp];
        e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }
        uint32_t idx = primeIdx[p];
        for (uint32_t t = 0; t < e; ++t) {
            parts[len++] = idx;
        }
    }

    /* Sort parts descending (insertion sort, len is small). */
    for (size_t i = 0; i < len; ++i) {
        for (size_t j = i + 1; j < len; ++j) {
            if (parts[j] > parts[i]) {
                uint32_t tmp = parts[i];
                parts[i] = parts[j];
                parts[j] = tmp;
            }
        }
    }

    if (is_conj_wilf(parts, len)) {
        if (cacheSize == cacheCap) {
            cacheCap = cacheCap ? cacheCap << 1 : 64;
            cache    = (seq_t *)realloc(cache, sizeof(seq_t) * cacheCap);
        }
        cache[cacheSize++] = (seq_t)x;
    }
}

/* Public entry point: a(n) returns the nth element of A383512. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* unused but safe */

    if (!initialized) {
        init_sieve();
        cacheCap  = 64;
        cache     = (seq_t *)malloc(sizeof(seq_t) * cacheCap);
        cacheSize = 0;

        /* a(1) is 1 (empty partition). */
        cache[cacheSize++] = (seq_t)1;
        currentX   = 1;                  /* next candidate will be 2 */
        initialized = true;
    }

    /* Extend cache until it contains at least n terms. */
    while (cacheSize < (size_t)n) {
        ++currentX;
        if (currentX > SIEVE_LIMIT) {
            /* Should not happen for the requested n. */
            return (seq_t)0;
        }
        process_candidate(currentX);
    }

    return cache[(size_t)n - 1];
}

/*
 * A373385  Numbers k for which A059975(k) is a multiple of 3, where A059975
 *          is fully additive with a(p) = p-1.
 * https://oeis.org/A373385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A373385: numbers k such that A059975(k) is a multiple of 3.
   A059975 is a fully additive function with a(p) = p‑1.
   This implementation searches for such numbers, checking each k on
   demand and caching the results. */
/*  The check uses only integer arithmetic. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Integer square root (exact floor) for 64‑bit unsigned values.     */
/* ------------------------------------------------------------------ */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = 0xFFFFFFFFULL, mid, result = 0;
    while (low <= high) {
        mid = (low + high) >> 1;
        uint64_t sq = mid * mid;
        if (sq <= n) { result = mid; low = mid + 1; }
        else          { high = mid - 1; }
    }
    return result;
}

/* ------------------------------------------------------------------ */
/* Prime table – built once up to a requested limit.                 */
/* ------------------------------------------------------------------ */
static uint32_t *prime_list  = NULL;
static size_t   prime_count  = 0;
static uint32_t prime_max    = 0;

static void ensure_primes_upto(uint64_t limit)
{
    if (limit <= prime_max) return;

    /* Allocate a simple sieve of size limit+1.  For small limits this is cheap. */
    size_t size = (size_t)limit + 1;
    bool *sieve = (bool *)calloc(size, 1);      /* 0 → prime, 1 → composite */
    if (!sieve) exit(1);

    for (size_t i = 2; i * i < size; ++i) {
        if (!sieve[i]) {
            for (size_t j = i * i; j < size; j += i) sieve[j] = true;
        }
    }

    /* Count primes and store them. */
    size_t cnt = 0;
    for (size_t i = 2; i < size; ++i) if (!sieve[i]) ++cnt;

    free(prime_list);
    prime_list = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!prime_list) exit(1);

    size_t idx = 0;
    for (size_t i = 2; i < size; ++i)
        if (!sieve[i]) prime_list[idx++] = (uint32_t)i;

    free(sieve);
    prime_count = cnt;
    prime_max   = (uint32_t)limit;
}

/* ------------------------------------------------------------------ */
/* Compute A059975(k) modulo 3 by elementary factorisation.         */
/* ------------------------------------------------------------------ */
static unsigned a_mod3(uint64_t k)
{
    if (k == 1) return 0;   /* A059975(1) = 0 */

    uint64_t n = k;
    unsigned sum = 0;

    uint64_t limit = isqrt_u64(n);
    ensure_primes_upto(limit);

    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * (uint64_t)p > n) break;
        if (n % p == 0) {
            unsigned exp = 0;
            while (n % p == 0) { n /= p; ++exp; }
            unsigned e_mod = exp % 3;
            if (e_mod) {
                unsigned contrib = ((p - 1) % 3) * e_mod;
                sum = (sum + contrib) % 3;
            }
        }
    }
    if (n > 1) {          /* remaining prime factor n, exponent 1 */
        unsigned contrib = ((n - 1) % 3);
        sum = (sum + contrib) % 3;
    }
    return sum;
}

/* ------------------------------------------------------------------ */
/* Main sequence function.                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache     = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static idx_t  last_k     = 0;   /* last integer examined */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 128;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
    }

    /* generate terms until we have at least n of them */
    while (cache_size < n) {
        ++last_k;
        if (a_mod3((uint64_t)last_k) == 0) {
            if (cache_size == cache_cap) {
                cache_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                cache = tmp;
            }
            cache[cache_size++] = (seq_t)last_k;
        }
    }

    return cache[n - 1];
}

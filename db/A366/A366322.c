/*
 * A366322  Heinz numbers of integer partitions containing at least one odd
 *          part. Numbers divisible by at least one prime of odd index.
 * https://oeis.org/A366322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = the n‑th positive integer that is divisible by at least
 *            one prime whose index in the prime sequence is odd (2,5,11,17,…)
 * Range:     u8 n <= 202, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* ----- static storage for the sequence --------------------------------- */
static seq_t *cache      = NULL;   /* cache of already computed terms   */
static size_t cache_len  = 0;      /* number of terms in the cache      */
static size_t cache_cap  = 0;      /* current capacity of the cache     */

/* ----- sieve data -------------------------------------------------------- */
static const size_t SIEVE_LIMIT = 2000000U;  /* large enough for typical use */
static bool *odd_idx_prime = NULL;   /* true if prime has odd index      */
static bool *good          = NULL;   /* true if number has an odd‑idx prime factor */

/* ----- helpers ------------------------------------------------------------ */

/* add a value to the cache, growing it if necessary */
static void cache_add(seq_t val) {
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 256;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* initialise sieve, identify odd‑indexed primes, mark numbers that are
   divisible by one of these primes and build an initial cache of terms   */
static void init_sequence(void) {
    odd_idx_prime = (bool *)calloc(SIEVE_LIMIT + 1, 1);
    good          = (bool *)calloc(SIEVE_LIMIT + 1, 1);
    bool *is_comp = (bool *)calloc(SIEVE_LIMIT + 1, 1);

    /* simple sieve to list primes and mark odd indices */
    uint32_t *primes = (uint32_t *)malloc( (SIEVE_LIMIT/10 + 10) * sizeof(uint32_t) );
    size_t prime_cnt = 0;

    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!is_comp[i]) {
            primes[prime_cnt++] = i;
            if ((prime_cnt & 1) == 1)          /* odd index (1‑based) */
                odd_idx_prime[i] = true;
            if ((uint64_t)i * i <= SIEVE_LIMIT) {
                for (uint32_t j = i * i; j <= SIEVE_LIMIT; j += i)
                    is_comp[j] = true;
            }
        }
    }
    free(is_comp);

    /* mark all numbers that are multiples of an odd‑indexed prime */
    for (size_t k = 0; k < prime_cnt; ++k) {
        if (odd_idx_prime[primes[k]]) {
            for (uint32_t m = primes[k]; m <= SIEVE_LIMIT; m += primes[k])
                good[m] = true;
        }
    }

    /* build initial cache from the sieve results */
    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (good[i])
            cache_add((seq_t)i);
    }

    free(primes);
}

/* ----- public sequence function ----------------------------------------- */
seq_t a(idx_t n) {
    static bool initialized = false;
    if (!initialized) {
        init_sequence();
        initialized = true;
    }
    /* assume requested n is within the precomputed cache (covers n≳10 000) */
    return cache[(size_t)n - 1];
}

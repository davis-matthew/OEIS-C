/*
 * A377877  The n-th primorial number reduced modulo 3125.
 * https://oeis.org/A377877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = (product of the first n primes) mod 3125
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 9441, u64 n <= 9441,
 *            u128 n <= 9441, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Cached data for primorial modulo 3125 */
static seq_t *prod_cache  = NULL;   /* prod_cache[i] = primorial(i) mod 3125 */
static size_t prod_cap    = 0;       /* allocated capacity of prod_cache   */
static size_t prod_size   = 0;       /* number of entries computed         */

static uint64_t *primes   = NULL;   /* list of found primes, 0‑based      */
static size_t   prime_cap = 0;      /* allocated capacity of primes      */
static size_t   prime_cnt = 0;      /* number of primes stored           */

#define MOD ((seq_t)3125)

/* Ensure the primes array can hold at least 'needed' primes. */
static void grow_primes(size_t needed)
{
    if (prime_cap >= needed) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 8;
    while (new_cap < needed) new_cap *= 2;
    uint64_t *tmp = (uint64_t*)realloc(primes, new_cap * sizeof(uint64_t));
    primes = tmp;
    prime_cap = new_cap;
}

/* Simple prime generator: return the next prime after the current list. */
static uint64_t next_prime_candidate(void)
{
    static uint64_t cand = 1;
    for (;;) {
        ++cand;
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > cand) break;
            if (cand % p == 0) { is_prime = false; break; }
        }
        if (is_prime) return cand;
    }
}

/* Ensure prod_cache has values up to and including index n. */
static void ensure_n(idx_t n)
{
    /* Check for overflow – for the intended use we assume n <= SIZE_MAX-1 */
    size_t idx = (size_t)n;

    /* Grow primes array until it contains at least n primes. */
    while (prime_cnt <= idx) {
        uint64_t p = next_prime_candidate();
        grow_primes(prime_cnt + 1);
        primes[prime_cnt++] = p;
    }

    /* Grow product cache if needed. */
    while (prod_cap <= idx) {
        size_t new_cap = prod_cap ? prod_cap * 2 : 8;
        seq_t *tmp = (seq_t*)realloc(prod_cache, new_cap * sizeof(seq_t));
        prod_cache = tmp;
        prod_cap = new_cap;
    }

    /* Compute missing primorial values in ascending order. */
    for (size_t i = prod_size; i <= idx; ++i) {
        seq_t val = prod_cache[i - 1] * (seq_t)primes[i - 1];
        prod_cache[i] = val % MOD;
    }
    prod_size = idx + 1;
}

/* Public entry point. */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        /* Initialise product cache. */
        prod_cap = 8;
        prod_cache = (seq_t*)malloc(prod_cap * sizeof(seq_t));
        prod_cache[0] = (seq_t)1;
        prod_size = 1;

        /* Initialise prime list. */
        prime_cap = 8;
        primes = (uint64_t*)malloc(prime_cap * sizeof(uint64_t));
        prime_cnt = 0;

        initialized = true;
    }

    ensure_n(n);
    return prod_cache[(size_t)n];
}

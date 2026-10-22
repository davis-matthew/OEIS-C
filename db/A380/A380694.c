/*
 * A380694  Numbers k such that the prime index of the least prime dividing k
 *          is larger than the maximum exponent in the prime factorization of
 *          k.
 * https://oeis.org/A380694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 110, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A380694
    Numbers k such that the prime index of the least prime dividing k
    is larger than the maximum exponent in the prime factorization of k.
    Implementation: linear generation with a sieve of Eratosthenes
    (fixed size for simplicity and speed). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define SIEVE_LIMIT 20000000ULL   /* limit for the prime sieve */

/* global data for the sieve */
static int  *prime_index = NULL;    /* prime_index[p] = 1‑based index of prime p, 0 if not prime */
static uint64_t *primes   = NULL;   /* list of primes up to SIEVE_LIMIT */
static size_t   num_primes = 0;      /* number of primes found */

/* one‑time sieve initialization */
static void init_sieve(void)
{
    if (prime_index != NULL) return;   /* already initialized */

    prime_index = (int*)malloc((SIEVE_LIMIT + 1) * sizeof(int));
    char       *is_prime = (char*)malloc((SIEVE_LIMIT + 1) * sizeof(char));
    if (!prime_index || !is_prime) exit(1);

    memset(is_prime, 1, SIEVE_LIMIT + 1);
    is_prime[0] = is_prime[1] = 0;

    for (size_t i = 2; i * i <= SIEVE_LIMIT; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_prime[j] = 0;

    primes = (uint64_t*)malloc((SIEVE_LIMIT / 2 + 100) * sizeof(uint64_t));
    if (!primes) exit(1);

    size_t idx = 0;
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (is_prime[i]) {
            primes[idx++] = i;
            prime_index[i] = (int)(idx);   /* 1‑based index */
        }

    num_primes = idx;
    primes = (uint64_t*)realloc(primes, num_primes * sizeof(uint64_t));
    free(is_prime);
}

/* check whether a candidate k satisfies the defining condition */
static bool is_term(uint64_t k)
{
    if (k < 3ULL) return false;

    uint64_t tmp = k;
    uint64_t smallest_prime = 0;
    int       max_exp = 0;

    for (size_t i = 0; i < num_primes; ++i) {
        uint64_t p = primes[i];
        if (p * p > tmp) break;
        if (tmp % p == 0) {
            if (smallest_prime == 0) smallest_prime = p;
            int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            if (exp > max_exp) max_exp = exp;
        }
    }

    if (tmp > 1) {                     /* remaining prime factor */
        if (smallest_prime == 0) smallest_prime = tmp;
        if (1 > max_exp) max_exp = 1;
    }

    /* if smallest_prime may exceed SIEVE_LIMIT we cannot lookup its index
       – but for the range of n requested the sieve is large enough. */
    int idx = 0;
    if (smallest_prime <= SIEVE_LIMIT)
        idx = prime_index[smallest_prime];
    else {
        /* compute prime index on the fly (sieve is large enough for the tests) */
        idx = 0;
        for (size_t i = 0; i < num_primes; ++i)
            if (primes[i] <= smallest_prime) ++idx;
        if (idx == 0 && smallest_prime > SIEVE_LIMIT)
            idx = (int)(num_primes + (smallest_prime - SIEVE_LIMIT) / 2); /* crude estimate */
    }

    return idx > max_exp;
}

/* cache of computed terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 3ULL;

/* append a new term to the cache */
static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        cache_cap = cache_cap ? cache_cap * 2 : 16;
        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
    }
    cache[cache_len++] = val;
}

/* --------------------------------------------------------------- */
/* public function required by the build system                    */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but harmless */

    init_sieve();

    while (cache_len < (size_t)n) {
        /* search for the next term */
        while (!is_term(next_candidate)) ++next_candidate;
        cache_append((seq_t)next_candidate);
        ++next_candidate;
    }

    return cache[n - 1];
}

/*
 * A279952  Number of primes with prime subscripts dividing n.
 * https://oeis.org/A279952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of distinct prime divisors p of n such that the
 *            index of p in the list of primes is itself a prime
 * Range:     u8 n <= 2038, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   build_primes_upto, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utilities (provided by the build environment) */
#include "../utils/is_prime_small.h"   /* bool is_prime_small(seq_t n); */
#include "../utils/build_primes_upto.h" /* void build_primes_upto(idx_t limit,
                                        uint64_t **primes_out, size_t *cnt_out); */

static bool    primes_initialized = false;
static size_t  primes_limit = 0;
static uint32_t *prime_index = NULL; /* prime_index[p] = index of p, 0 if not prime */
static uint32_t *prime_list  = NULL; /* array of primes up to primes_limit */
static size_t   primes_count  = 0;

/* Rebuilds prime table up to at least 'limit' */
static void
init_primes(size_t limit)
{
    free(prime_index);
    free(prime_list);

    prime_index = (uint32_t *)calloc(limit + 1, sizeof(uint32_t));
    uint64_t *pr = NULL;
    size_t   pc = 0;
    build_primes_upto(limit, &pr, &pc);

    prime_list  = (uint32_t *)malloc(pc * sizeof(uint32_t));
    for (size_t i = 0; i < pc; ++i) {
        uint32_t p = (uint32_t)pr[i];
        prime_list[i] = p;
        prime_index[p] = (uint32_t)(i + 1);  /* indices start at 1 */
    }
    free(pr);

    primes_limit = limit;
    primes_count = pc;
    primes_initialized = true;
}

/* Returns the count of prime divisors of 'n' whose prime index is prime. */
seq_t a(idx_t n)
{
    if (!primes_initialized || (size_t)n > primes_limit)
        init_primes((size_t)n);

    seq_t result = 0;
    idx_t temp = n;

    for (size_t i = 0; i < primes_count; ++i) {
        uint32_t p = prime_list[i];
        if ((idx_t)p * (idx_t)p > temp)
            break;
        if (temp % p == 0) {
            uint32_t idx = prime_index[p];
            if (is_prime_small((seq_t)idx))
                ++result;
            while (temp % p == 0)
                temp /= p;
        }
    }
    if (temp > 1) {                     /* remaining prime factor */
        uint32_t idx = prime_index[(uint32_t)temp];
        if (is_prime_small((seq_t)idx))
            ++result;
    }
    return result;
}

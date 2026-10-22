/*
 * A324370  Product of all primes p not dividing n such that the sum of the
 *          base-p digits of n is at least p, or 1 if no such prime exists.
 * https://oeis.org/A324370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = product of all primes p not dividing n such that * the
 *            sum of the base‑p digits of n is at least p; 1 if no such prime
 *            exists. * This implementation generates the necessary list of
 *            primes up to n * using a simple sieve and then evaluates the
 *            product directly for each call
 * Range:     u8 n <= 24, u16 n <= 60, u32 n <= 180, u64 n <= 420,
 *            u128 n <= 1800, big any n
 * Helpers:   digit_sum_base_p
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_sum_base_p.h"

/* Prime list stored in a dynamically allocated array. */
static uint64_t *prime_list = NULL;   /* primes up to prime_limit */
static size_t   prime_cnt   = 0;      /* number of primes generated */

static uint64_t prime_limit = 0;      /* current maximum n for which primes are generated */

/* Extend the prime list to contain all primes <= limit.
 * A fresh sieve is performed each time the limit increases. */
static void extend_primes_to(uint64_t limit)
{
    if (limit <= prime_limit)
        return;

    /* free any previously allocated prime array */
    free(prime_list);
    prime_list = NULL;
    prime_cnt  = 0;
    prime_limit = limit;

    /* Sieve of Eratosthenes */
    size_t size = (size_t)(limit + 1);
    bool *sieve = (bool *)malloc(size);
    if (!sieve)
        return;           /* memory allocation failure – rare in the test context */

    for (size_t i = 0; i < size; ++i)
        sieve[i] = true;
    if (limit >= 0) sieve[0] = false;
    if (limit >= 1) sieve[1] = false;

    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (sieve[i]) {
            for (uint64_t j = i * i; j <= limit; j += i)
                sieve[j] = false;
        }
    }

    /* Count primes to allocate array once. */
    for (uint64_t i = 2; i <= limit; ++i)
        if (sieve[i])
            ++prime_cnt;
    prime_list = (uint64_t *)malloc(prime_cnt * sizeof(uint64_t));
    if (!prime_list) {
        free(sieve);
        prime_limit = 0;
        return;
    }

    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (sieve[i])
            prime_list[idx++] = i;

    free(sieve);
}

/* The required OEIS sequence term. */
seq_t a(idx_t n)
{
    if (n == 0)          /* n=0 is outside the defined offset; return 0 safely */
        return 0;

    extend_primes_to((uint64_t)n);      /* ensure primes up to n are available   */
    seq_t prod = 1;                      /* 1 when no qualifying primes exist     */

    for (size_t i = 0; i < prime_cnt && prime_list[i] <= (uint64_t)n; ++i) {
        uint64_t p = prime_list[i];
        if ((n % p) == 0)           /* skip primes that divide n           */
            continue;

        seq_t sum = digit_sum_base_p(n, p);
        if (sum >= (seq_t)p)
            prod *= (seq_t)p;        /* multiply qualified prime           */
    }

    return prod;
}

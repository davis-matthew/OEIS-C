/*
 * A369610  a(n) is the number of nonprime numbers < prime(n) which are not
 *          equal to twice a prime.
 * https://oeis.org/A369610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) - pi(prime(n)) - pi((prime(n)-1)/2)
 * Range:     u8 n <= 73, u16 n <= 7582, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (i.e. number of non‑prime < prime(n) that are not twice a prime) */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- Prime sieve infrastructure ---------- */
static uint64_t *prime_list = NULL;     /* primes as uint64_t, 1‑based if need */
static size_t   prime_count = 0;        /* number of primes stored          */
static size_t   prime_cap   = 0;        /* current capacity of prime_list   */
static size_t   sieve_limit = 0;        /* upper bound of current sieve    */

/* build all primes up to *limit* (inclusive) */
static void refresh_primes(size_t limit)
{
    /* free old sieve data */
    free(prime_list);
    prime_list = NULL;
    prime_count = 0;
    prime_cap   = 0;

    /* simple sieve of Eratosthenes (limit <= 1e7 for the expected n <= 1e4) */
    uint8_t *is_prime = (uint8_t*)malloc(limit + 1);
    if (!is_prime) abort();

    for (size_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    if (limit >= 0) is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;

    for (size_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;

    /* collect primes into prime_list */
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) {
            if (prime_count == prime_cap) {
                prime_cap = prime_cap == 0 ? 16 : prime_cap * 2;
                prime_list = (uint64_t*)realloc(prime_list, prime_cap * sizeof(uint64_t));
                if (!prime_list) abort();
            }
            prime_list[prime_count++] = (uint64_t)i;
        }

    free(is_prime);
    sieve_limit = limit;
}

/* ensure that there are at least *target* primes in prime_list */
static void ensure_primes(idx_t target)
{
    while (prime_count < (size_t)target) {
        size_t new_limit = sieve_limit == 0 ? 20 : sieve_limit * 2;
        refresh_primes(new_limit);
    }
}

/* binary search: number of primes <= x  (x fits in uint64_t) */
static size_t count_primes_leq(uint64_t x)
{
    size_t left = 0, right = prime_count;
    while (left < right) {
        size_t mid = (left + right) >> 1;
        if (prime_list[mid] <= x) left = mid + 1;
        else                    right = mid;
    }
    return left;                 /* number of primes <= x */
}

/* ---------- public sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset starts at 1; undefined for n=0 */

    ensure_primes(n);                /* guarantee prime(n) is available */

    uint64_t p = prime_list[(size_t)(n-1)];   /* p = prime(n)           */
    uint64_t half = (p - 1) / 2;              /* floor((p-1)/2)         */
    size_t pi_half = count_primes_leq(half);   /* pi(floor((p-1)/2))    */

    /* a(n) = prime(n) - pi(prime(n)) - pi((prime(n)-1)/2) */
    /* pi(prime(n)) = n                                 */
    return (seq_t)p - (seq_t)n - (seq_t)pi_half;
}

/*
 * A248371  Number of primes in Breusch's interval [n; 9(n+3)/8].
 * https://oeis.org/A248371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the definition: 
   a(n) = number of primes in the interval [n, 9(n+3)/8]  (inclusive).
   Uses a simple static sieve that is extended as needed.  */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* Static state for the sieve */
static idx_t sieve_limit = 0;
static bool *is_prime = NULL;
static uint64_t *primes = NULL;
static size_t prime_count = 0;

/* Extend or build the sieve up to `limit`.  */
static void ensure_sieve(idx_t limit) {
    if (limit <= sieve_limit && is_prime != NULL)
        return;                      /* already sufficient */

    /* Free previous data */
    free(is_prime);
    free(primes);

    sieve_limit = limit;
    /* Allocate memory, casting the void* result. */
    is_prime = (bool *)calloc(sieve_limit + 1, sizeof(bool));
    if (!is_prime) abort();

    /* Initialize all entries as prime */
    if (sieve_limit >= 0) is_prime[0] = false;
    if (sieve_limit >= 1) is_prime[1] = false;
    for (idx_t i = 2; i <= sieve_limit; ++i)
        is_prime[i] = true;

    /* Eratosthenes sieve */
    idx_t r = (idx_t)sqrt((double)sieve_limit);
    for (idx_t p = 2; p <= r; ++p) {
        if (!is_prime[p]) continue;
        for (idx_t j = p * p; j <= sieve_limit; j += p)
            is_prime[j] = false;
    }

    /* Count primes and collect them */
    prime_count = 0;
    for (idx_t i = 2; i <= sieve_limit; ++i)
        if (is_prime[i]) ++prime_count;

    primes = (uint64_t *)malloc(prime_count * sizeof(uint64_t));
    if (!primes) abort();
    size_t idx = 0;
    for (idx_t i = 2; i <= sieve_limit; ++i)
        if (is_prime[i]) primes[idx++] = i;
}

/* Binary search: number of primes <= x.  */
static seq_t count_primes_leq(idx_t x) {
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (seq_t)lo;
}

seq_t a(idx_t n) {
    /* Compute the upper bound of the interval. */
    idx_t upper = 9 * (n + 3) / 8;

    /* Build or extend the sieve up to `upper`. */
    ensure_sieve(upper);

    /* Count primes in the interval [n, upper]. */
    seq_t pi_upper = count_primes_leq(upper);
    seq_t pi_before = (n == 0) ? (seq_t)0 : count_primes_leq(n - 1);

    return pi_upper - pi_before;
}

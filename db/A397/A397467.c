/*
 * A397467  a(n) = prime(2n + floor(n*sqrt(2))).
 * https://oeis.org/A397467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(2*n + floor(n*sqrt(2)))
 * Range:     u8 n <= 16, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

static const long double SQRT2 = 1.414213562373095048801688724209698078569671875376948073176;

static idx_t *prime_list = NULL;
static size_t prime_cap = 0;
static size_t prime_count = 0;
static idx_t sieve_limit = 0;

/* Estimate an upper bound for the k-th prime. */
static idx_t estimate_limit(idx_t k)
{
    if (k < 6) return 15;
    double dk = (double)k;
    double est = dk * (log(dk) + log(log(dk))) + 3.0;
    return (idx_t)est;
}

/* Ensure that prime_list contains at least k primes (1 ≤ k). */
static void ensure_primes(idx_t k)
{
    if (prime_count >= (size_t)k) return;
    idx_t limit = estimate_limit(k);
    if (limit <= sieve_limit) limit = sieve_limit + 1;  /* increase if needed */

    /* Allocate sieve */
    uint8_t *sieve = (uint8_t *)malloc((size_t)(limit + 1));
    if (!sieve) exit(EXIT_FAILURE);
    for (idx_t i = 0; i <= limit; ++i) sieve[i] = 1;
    sieve[0] = sieve[1] = 0;

    /* Sieve of Eratosthenes */
    for (idx_t i = 2; i * i <= limit; ++i)
        if (sieve[i])
            for (idx_t j = i * i; j <= limit; j += i)
                sieve[j] = 0;

    /* Count primes */
    size_t pcount = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (sieve[i]) ++pcount;

    /* Allocate or grow prime list */
    idx_t *tmp = (idx_t *)realloc(prime_list, pcount * sizeof(idx_t));
    if (!tmp) { free(sieve); exit(EXIT_FAILURE); }
    prime_list = tmp;
    prime_cap = pcount;

    /* Fill prime list */
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (sieve[i]) prime_list[idx++] = i;

    prime_count = pcount;
    sieve_limit = limit;
    free(sieve);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for offset < 1, but safe */
    long double val = (long double)n * SQRT2;
    idx_t m = (idx_t)val;           /* floor(n*sqrt(2)) */
    idx_t k = 2 * n + m;            /* index of required prime */
    ensure_primes(k);
    return (seq_t)prime_list[(size_t)(k - 1)];
}

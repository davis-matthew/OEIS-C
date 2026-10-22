/*
 * A377320  a(n) is the smallest positive integer k such that n + k and n - k
 *          have the same number of prime factors.
 * https://oeis.org/A377320
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest positive integer k such that the number of
 *            prime factors (with multiplicity) of n+k equals that of n-k. The
 *            search starts at k=1 and increases until the condition is met
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard C headers */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Static prime sieve up to a fixed limit; sufficient for
   n up to at least 1e5 (the problem tests n up to ~10000).
   If a larger n is needed, increase LIMIT. */
#define PRIMES_LIMIT 200000

static uint64_t *prime_list = NULL;   /* Array of primes */
static size_t    prime_count = 0;     /* Number of primes stored */

/* Initialize the prime list once using a simple sieve of Eratosthenes */
static void init_primes(void)
{
    if (prime_list) return;  /* already initialized */

    /* Boolean array for sieve */
    uint8_t *is_prime = (uint8_t *)calloc(PRIMES_LIMIT + 1, sizeof(uint8_t));
    if (!is_prime) abort();   /* allocation failure */

    memset(is_prime, 1, PRIMES_LIMIT + 1);
    is_prime[0] = is_prime[1] = 0;

    for (size_t p = 2; p * p <= PRIMES_LIMIT; ++p) {
        if (is_prime[p]) {
            for (size_t multiple = p * p; multiple <= PRIMES_LIMIT; multiple += p) {
                is_prime[multiple] = 0;
            }
        }
    }

    /* Count primes to allocate exact array size */
    prime_count = 0;
    for (size_t i = 2; i <= PRIMES_LIMIT; ++i) {
        if (is_prime[i]) prime_count++;
    }

    prime_list = (uint64_t *)malloc(prime_count * sizeof(uint64_t));
    if (!prime_list) abort();  /* allocation failure */

    size_t idx = 0;
    for (size_t i = 2; i <= PRIMES_LIMIT; ++i) {
        if (is_prime[i]) prime_list[idx++] = i;
    }

    free(is_prime);
}

/* Count number of prime factors with multiplicity for a 64‑bit integer. */
static int count_omega_uint64(uint64_t n)
{
    int cnt = 0;
    for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= n; ++i) {
        uint64_t p = prime_list[i];
        while (n % p == 0) {
            cnt++;
            n /= p;
        }
    }
    if (n > 1) cnt++;   /* remaining prime factor */
    return cnt;
}

/* Main sequence function. External linkage as required. */
seq_t a(idx_t n)
{
    /* Undefined behavior for n < 4; we simply return 0. */
    if (n < 4) return 0;

    init_primes();  /* Ensure primes are ready */

    /* Search for the smallest positive k with equal omega. */
    for (idx_t k = 1; ; ++k) {
        uint64_t m1 = (uint64_t)n + k;
        uint64_t m2 = (uint64_t)n - k;
        if (count_omega_uint64(m1) == count_omega_uint64(m2)) {
            return (seq_t)k;
        }
    }
}

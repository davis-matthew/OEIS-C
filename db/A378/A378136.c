/*
 * A378136  a(n) = p(2*n*p(n)), where p(x) = least prime > x.
 * https://oeis.org/A378136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(2*n*p(n)) where p(x) = least prime greater than x
 * Range:     u8 n <= 10, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Prime sieve for small primes up to 50000.  These primes are used to
   test primality of numbers up to (2*10^4*10^5) ≈ 2e9. */
static uint32_t *primes = NULL;      /* array of primes */
static size_t prime_count = 0;       /* number of primes */

/* Initialise the prime list once. */
static void init_primes(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    const uint32_t LIMIT = 50000;
    bool *is_prime = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_prime) exit(1);  /* abort if allocation fails */

    for (uint32_t i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (uint32_t p = 2; p * p <= LIMIT; ++p) {
        if (is_prime[p]) {
            for (uint32_t q = p * p; q <= LIMIT; q += p) {
                is_prime[q] = false;
            }
        }
    }

    /* Count primes */
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (is_prime[i]) ++prime_count;
    }

    primes = (uint32_t *)malloc(prime_count * sizeof(uint32_t));
    if (!primes) exit(1);

    size_t idx = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (is_prime[i]) {
            primes[idx++] = i;
        }
    }

    free(is_prime);
}

/* Test whether n is prime using the precomputed small primes. */
static bool is_prime_small(uint64_t n)
{
    if (n < 2) return false;
    if (n <= 3) return true;
    /* Check divisibility by precomputed primes up to sqrt(n). */
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * (uint64_t)p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Find the least prime greater than start (start may be 0). */
static uint64_t next_prime(uint64_t start)
{
    init_primes();
    uint64_t candidate = start + 1;
    while (!is_prime_small(candidate)) ++candidate;
    return candidate;
}

/* The sequence generator */
seq_t a(idx_t n)
{
    /* Convert idx_t to uint64_t for arithmetic. */
    uint64_t nn = (uint64_t)n;
    uint64_t pn = next_prime(nn);          /* p(n)  */
    __int128 m128 = (__int128)nn * (__int128)(2 * pn);
    uint64_t m = (uint64_t)m128;           /* 2*n*p(n)  */
    uint64_t pm = next_prime(m);           /* p(2*n*p(n)) */
    return (seq_t)pm;
}

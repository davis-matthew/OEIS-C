/*
 * A351519  a(n) = tau(n) + pi(n) - omega(n).
 * https://oeis.org/A351519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = tau(n) + pi(n) - omega(n)
 * Range:     u8 n <= 1439, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 92 terms through n = 9013 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Count divisors of a 64‑bit integer */
static uint64_t divisor_count_u64(uint64_t n)
{
    uint64_t tau = 1;
    uint64_t exp;

    /* factor 2 */
    exp = 0;
    while ((n & 1ULL) == 0) {
        n >>= 1;
        ++exp;
    }
    if (exp) tau *= (exp + 1);

    /* factor odd primes */
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            tau *= (exp + 1);
        }
    }

    /* remaining prime factor */
    if (n > 1) tau *= 2;

    return tau;
}

/* Return the prime counting function pi(n) for any n ≥ 0
 * by sieving up to n on each call. */
static seq_t compute_pi(idx_t n)
{
    if (n < 2)
        return 0;

    /* Allocate a boolean array for the sieve */
    unsigned char *is_prime = (unsigned char *)malloc((n + 1) * sizeof(unsigned char));
    if (!is_prime)
        abort();                     /* allocation failed */

    /* Initialise all entries as prime except 0 and 1 */
    for (idx_t i = 0; i <= n; ++i)
        is_prime[i] = 1;
    is_prime[0] = 0;
    is_prime[1] = 0;

    /* Classic sieve of Eratosthenes */
    for (idx_t p = 2; p * p <= n; ++p) {
        if (is_prime[p]) {
            for (idx_t q = p * p; q <= n; q += p)
                is_prime[q] = 0;
        }
    }

    /* Count primes up to n */
    seq_t count = 0;
    for (idx_t i = 2; i <= n; ++i)
        if (is_prime[i])
            ++count;

    free(is_prime);
    return count;
}

/* Count distinct prime factors of an unsigned 64-bit integer n.
   The result fits in a signed int for all n that may be passed. */
static int count_distinct_primes(uint64_t n)
{
    if (n <= 1)
        return 0;

    int cnt = 0;
    /* Handle factor 2 separately. */
    if (n % 2 == 0) {
        cnt++;
        while (n % 2 == 0)
            n /= 2;
    }
    /* Test odd divisors up to sqrt(n). */
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            cnt++;
            while (n % p == 0)
                n /= p;
        }
    }
    if (n > 1)   /* Remaining prime factor. */
        cnt++;
    return cnt;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1. */
    if (n < 1) {
        return 0;           /* undefined for n = 0, return 0 safely */
    }

    /* Classically: tau(n) = number of positive divisors of n */
    seq_t tau = (seq_t)divisor_count_u64((uint64_t)n);

    /* pi(n) = number of primes ≤ n */
    seq_t pi = compute_pi(n);

    /* omega(n) = number of distinct prime factors of n */
    seq_t omega = (seq_t)count_distinct_primes((uint64_t)n);

    /* a(n) = tau + pi - omega */
    return tau + pi - omega;
}

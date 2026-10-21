/*
 * A255472  Number of decompositions of 2n into sums of two primes p <= q such
 *          that one or both p and q are elements of A023201.
 * https://oeis.org/A255472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of decompositions of 2n into primes p ≤ q * where
 *            at least one of p or q is a sexy prime (p+6 prime)
 * Range:     u8 n <= 7916, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Static data for the sieve */
static idx_t sieve_limit = 0;
static bool *is_prime   = NULL;
static bool *is_sexy    = NULL;

/* Build or extend a sieve up to "limit" inclusive.
 * The array is grown whenever a higher limit is requested. */
static void build_sieve(idx_t limit)
{
    if (sieve_limit >= limit) return;      /* already enough */

    /* release old arrays */
    free(is_prime);
    free(is_sexy);

    is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    is_sexy  = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_prime || !is_sexy) exit(1);

    /* initialize */
    for (idx_t i = 0; i <= limit; ++i) {
        is_prime[i] = true;
        is_sexy[i]  = false;
    }
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    /* sieve of Eratosthenes */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    /* mark sexy primes (p such that p+6 is prime) */
    for (idx_t i = 2; i + 6 <= limit; ++i) {
        if (is_prime[i] && is_prime[i + 6])
            is_sexy[i] = true;
    }

    sieve_limit = limit;
}

/* Main function implementing the sequence */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                   /* safety for offset handling */
    idx_t two_n = 2 * n;
    /* Ensure we can test primality of p+6 and q+6 when needed. */
    build_sieve(two_n + 6);

    seq_t count = 0;
    for (idx_t p = 2; p <= two_n / 2; ++p) {
        if (!is_prime[p]) continue;
        idx_t q = two_n - p;
        if (!is_prime[q]) continue;
        if (is_sexy[p] || is_sexy[q]) ++count;
    }
    return count;
}

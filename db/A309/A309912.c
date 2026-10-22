/*
 * A309912  a(n) = Product_{p prime, p <= n} floor(n/p).
 * https://oeis.org/A309912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{prime p ≤ n} floor(n/p)
 * Range:     u8 n <= 17, u16 n <= 34, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Compute a(n) as defined in OEIS A309912.
 * For n < 2 there are no primes ≤ n, so the empty product is 1. */
seq_t a(idx_t n)
{
    /* Shortcut for the small cases */
    if (n < 2)
        return (seq_t)1;

    /* Allocate a byte array to mark primes up to n. */
    idx_t size = n + 1;                     /* we need indices 0..n inclusive */
    char *is_prime = (char *)malloc(size);
    if (!is_prime)                          /* allocation failed – return 1 */
        return (seq_t)1;

    /* Initialise the sieve */
    is_prime[0] = 0;
    is_prime[1] = 0;
    for (idx_t i = 2; i < size; ++i)
        is_prime[i] = 1;

    /* Classic sieve of Eratosthenes */
    for (idx_t i = 2; i * i <= n; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= n; j += i)
                is_prime[j] = 0;
        }
    }

    /* Multiply the floor(n/p) values over all primes p ≤ n. */
    seq_t prod = 1;
    for (idx_t p = 2; p <= n; ++p) {
        if (is_prime[p]) {
            prod *= (seq_t)(n / p); /* integer division gives floor(n/p) */
        }
    }

    free(is_prime);
    return prod;
}

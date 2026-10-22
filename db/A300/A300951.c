/*
 * A300951  a(n) = Product_{j=1..floor(n/2)} p(j) where p(j) = j if j is prime
 *          else 1.
 * https://oeis.org/A300951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{j=1..⌊n/2⌋} (j if j is prime else 1)
 * Range:     u8 n <= 21, u16 n <= 33, u32 n <= 57, u64 n <= 105,
 *            u128 n <= 205, big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* m = floor(n/2) */
    idx_t m = n >> 1;

    /* For m < 2 there is no prime ≤ m, product is 1 */
    if (m < 2)
        return (seq_t)1;

    /* Sieve of Eratosthenes up to m.  Allocate with a cast for C++ */
    uint8_t *is_prime = (uint8_t *)malloc((size_t)(m + 1));
    if (!is_prime)
        return (seq_t)0;          /* memory failure – unlikely for test data   */

    /* Initialise all entries to prime */
    for (idx_t i = 0; i <= m; ++i)
        is_prime[i] = 1;
    is_prime[0] = 0;
    is_prime[1] = 0;

    /* Mark composites */
    for (idx_t p = 2; p <= m; ++p)
    {
        if (!is_prime[p])
            continue;
        /* Avoid overflow in p*p */
        if (p > m / p)
            continue;
        for (idx_t j = p * p; j <= m; j += p)
            is_prime[j] = 0;
    }

    /* Compute the product of all primes ≤ m */
    seq_t prod = (seq_t)1;
    for (idx_t i = 2; i <= m; ++i)
        if (is_prime[i])
            prod = seq_mul(prod, (seq_t)i);

    free(is_prime);
    return prod;
}

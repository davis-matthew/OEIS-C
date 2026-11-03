/*
 * A353475  a(n) = 1 if n is a semiprime of the form p * q, where p and q are
 *          primes with p^2 < q, otherwise 0.
 * https://oeis.org/A353475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n = p * q with p and q primes and p² < q, 0
 *            otherwise. The function implements a simple prime test and
 *            checks the semiprime condition directly
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 137 terms through n = 9691 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Numbers less than 10 cannot satisfy p² < q with p, q primes. */
    if (n < 6)
        return 0;

    /* Find the smallest prime factor p of n. */
    idx_t p = 0;
    for (idx_t d = 2; d * d <= n; ++d) {
        if (n % d == 0) {
            p = d;
            break;
        }
    }
    /* If no divisor found, n is prime. */
    if (p == 0)
        return 0;

    idx_t q = n / p;

    /* n must have exactly two prime factors; check that q is prime. */
    if (!is_prime(q))
        return 0;

    /* Check the inequality p² < q. */
    if (p * p < q)
        return 1;
    return 0;
}

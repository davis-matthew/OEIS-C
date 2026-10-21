/*
 * A286634  Numerator of the ratio of alternate consecutive prime gaps:
 *          Numerator ((prime(n + 3) - prime(n + 2))/(prime(n + 1) -
 *          prime(n))).
 * https://oeis.org/A286634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator((prime(n+3)-prime(n+2))/(prime(n+1)-prime(n)))
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the numerator of the reduced fraction formed by the ratio of */
/* two consecutive prime gaps, where the gaps are taken for primes index */
/* n, n+1, n+2, n+3. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Retrieve the required primes.
       get_prime(k) returns the k-th prime (1‑based). */
    uint32_t p1 = get_prime(n);
    uint32_t p2 = get_prime(n + 1);
    uint32_t p3 = get_prime(n + 2);
    uint32_t p4 = get_prime(n + 3);

    /* Compute the two prime gaps. */
    seq_t g1 = (seq_t)p2 - (seq_t)p1; /* gap between prime n and prime n+1 */
    seq_t g2 = (seq_t)p4 - (seq_t)p3; /* gap between prime n+2 and prime n+3 */

    /* Reduce the fraction g2/g1 to lowest terms. */
    seq_t d = gcd(g1, g2);
    return g2 / d;
}

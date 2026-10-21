/*
 * A274382  a(n) = gcd(n, n*(n+1)/2 - sigma(n)).
 * https://oeis.org/A274382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = gcd(n, n*(n+1)/2 - sigma(n))
 * Range:     u8 n <= 27, u16 n <= 5452, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the utilities that provide sigma_of and gcd. */
#include "../utils/sigma_of.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Compute the triangular number T = n*(n+1)/2. */
    seq_t tri = (seq_t)n * ((seq_t)n + 1) / 2;

    /* Sum of divisors sigma(n). */
    seq_t sum = sigma_of(n);

    /* Antisigma = T - sigma(n). */
    seq_t antisigma = tri - sum;

    /* Return gcd(n, antisigma). */
    return gcd((seq_t)n, antisigma);
}

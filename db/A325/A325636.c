/*
 * A325636  a(n) = gcd(2n, sigma(n)).
 * https://oeis.org/A325636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(2n, sigma(n))
 * Range:     u8 n <= 208, u16 n <= 9981, u32 n <= 9981, u64 n <= 9981,
 *            u128 n <= 9981, big any n
 * Helpers:   gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = gcd(2n, sum of divisors of n) using helper utilities. */

#include "../utils/sigma_of.h"
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* Ensure n is at least 1; behavior for n == 0 is undefined but allowed. */
    seq_t sigma_n = sigma_of(n);
    seq_t two_n   = (seq_t)n * 2;
    return gcd(sigma_n, two_n);
}

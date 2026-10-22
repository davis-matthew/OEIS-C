/*
 * A325964  a(n) = 1 if n and sigma(n) are relatively prime, 0 otherwise,
 *          where sigma(n) = sum of divisors of n, A000203; Characteristic
 *          function of A014567.
 * https://oeis.org/A325964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if gcd(n, sigma(n)) == 1, 0 otherwise
 * Range:     u8 n <= 4552, u16 n <= 9686, u32 n <= 9686, u64 n <= 9686,
 *            u128 n <= 9686, big any n
 * Helpers:   gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    seq_t sigma = sigma_of(n);
    seq_t g = gcd((seq_t)n, sigma);
    return g == 1 ? 1 : 0;
}

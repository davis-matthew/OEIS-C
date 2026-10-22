/*
 * A370690  Denominator of sigma(phi(n))/phi(sigma(n)), where sigma is the sum
 *          of the divisors function and phi is the Euler totient function.
 * https://oeis.org/A370690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = denominator of sigma(phi(n))/phi(sigma(n))
 * Range:     u8 n <= 362, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/sigma_of.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    seq_t phi_n    = phi(n);
    seq_t sigma_phi = sigma_of((idx_t)phi_n);
    seq_t sigma_n  = sigma_of(n);
    seq_t phi_sigma = phi((idx_t)sigma_n);
    seq_t g        = gcd(sigma_phi, phi_sigma);
    return phi_sigma / g;
}

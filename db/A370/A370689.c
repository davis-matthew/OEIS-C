/*
 * A370689  Numerator of sigma(phi(n))/phi(sigma(n)), where sigma is the sum
 *          of the divisors function and phi is the Euler totient function.
 * https://oeis.org/A370689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator of σ(φ(n)) / φ(σ(n))
 * Range:     u8 n <= 161, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/calc_phi.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* phi(n) */
    seq_t phi_n = calc_phi(n);

    /* σ(φ(n)) – cast the value back to idx_t for the sigma_of function */
    idx_t phi_n_idx = (idx_t)phi_n;
    seq_t sigma_phi_n = sigma_of(phi_n_idx);

    /* σ(n) and φ(σ(n)) */
    seq_t sigma_n   = sigma_of(n);
    idx_t sigma_n_idx = (idx_t)sigma_n;
    seq_t phi_sigma_n = calc_phi(sigma_n_idx);

    /* numerator = σ(φ(n)) / gcd(σ(φ(n)), φ(σ(n))) */
    seq_t g = gcd(sigma_phi_n, phi_sigma_n);
    return sigma_phi_n / g;
}

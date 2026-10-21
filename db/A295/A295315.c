/*
 * A295315  a(n) = phi(sigma(n)) / gcd(sigma(n), phi(sigma(n))).
 * https://oeis.org/A295315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(sigma(n)) / gcd(sigma(n), phi(sigma(n)))
 * Range:     u8 n <= 143, u16 n <= 8752, u32 n <= 8752, u64 n <= 8752,
 *            u128 n <= 8752, big any n
 * Helpers:   calc_phi, gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/sigma_of.h"
#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    /* sigma(n) */
    seq_t sigma = sigma_of(n);
    /* phi(sigma(n)) */
    seq_t phi_sigma = calc_phi(sigma);
    /* gcd(sigma, phi_sigma) */
    seq_t g = gcd(sigma, phi_sigma);
    /* a(n) = phi_sigma / g */
    return phi_sigma / g;
}

/*
 * A295314  a(n) = sigma(n) / gcd(sigma(n), phi(sigma(n))).
 * https://oeis.org/A295314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n) / gcd(sigma(n), phi(sigma(n)))
 * Range:     u8 n <= 143, u16 n <= 9708, u32 n <= 9708, u64 n <= 9708,
 *            u128 n <= 9708, big any n
 * Helpers:   calc_phi, seq_gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/calc_phi.h"
#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    seq_t sigma = sigma_of(n);
    seq_t phi_sigma = calc_phi(sigma);
    seq_t g = seq_gcd(sigma, phi_sigma);
    return sigma / g;
}

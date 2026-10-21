/*
 * A295311  a(n) = n / A295310(n) = n / gcd(n, phi(sigma(n))).
 * https://oeis.org/A295311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n / gcd(n, phi(sigma(n)))
 * Range:     u8 n <= 255, u16 n <= 9615, u32 n <= 9615, u64 n <= 9615,
 *            u128 n <= 9615, big any n
 * Helpers:   calc_phi, gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/gcd.h"
#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    /* sum of divisors of n */
    seq_t sigma = sigma_of(n);
    /* Euler's totient of sigma */
    seq_t phi_val = calc_phi(sigma);
    /* gcd(n, phi(sigma)) */
    seq_t g = gcd((seq_t)n, phi_val);
    /* return n / g */
    return (seq_t)n / g;
}

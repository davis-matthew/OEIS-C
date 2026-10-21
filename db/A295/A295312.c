/*
 * A295312  a(n) = A062401(n) / A295310(n) = phi(sigma(n)) / gcd(n,
 *          phi(sigma(n))).
 * https://oeis.org/A295312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(sigma(n)) / gcd(n, phi(sigma(n)))
 * Range:     u8 n <= 379, u16 n <= 9639, u32 n <= 9639, u64 n <= 9639,
 *            u128 n <= 9639, big any n
 * Helpers:   calc_phi, gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/calc_phi.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* the sequence is defined for n >= 1 */

    seq_t sigma = sigma_of(n);
    seq_t phi_val = calc_phi(sigma);
    seq_t g = gcd((seq_t)n, phi_val);
    return phi_val / g;
}

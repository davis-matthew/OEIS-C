/*
 * A289412  a(n) = denominator of (sigma(n) / phi(n)).
 * https://oeis.org/A289412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = phi(n) / gcd(sigma(n), phi(n))
 * Range:     u8 n <= 179, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/sigma_of.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    seq_t sigma_val = sigma_of(n);
    seq_t phi_val   = phi(n);
    seq_t g         = gcd(sigma_val, phi_val);
    return phi_val / g;
}

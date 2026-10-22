/*
 * A386402  a(n) = denominator(sigma(n)*phi(n)/n^2).
 * https://oeis.org/A386402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = denominator(sigma(n)*phi(n)/n^2)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/phi.h"
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    seq_t sigma_n = sigma_of(n);
    seq_t phi_n   = phi(n);
    seq_t denom   = (seq_t)n * (seq_t)n;
    seq_t numer   = sigma_n * phi_n;
    seq_t g       = gcd(numer, denom);
    return denom / g;
}

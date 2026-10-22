/*
 * A353643  The greatest common divisor of phi(n) and phi(sigma(n)).
 * https://oeis.org/A353643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(phi(n), phi(sigma(n))) // matches the OEIS data
 * Range:     u8 n <= 469, u16 n <= 8234, u32 n <= 8234, u64 n <= 8234,
 *            u128 n <= 8234, big any n
 * Helpers:   gcd, phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/phi.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n) {
    /* φ(n) */
    seq_t phi_n = phi(n);

    /* σ(n) */
    seq_t sigma_n = sigma_of(n);

    /* φ(σ(n)) – σ(n) fits in idx_t for the range of n we use.
       Cast safely because σ(n) < 2^64 for n up to 10000.   */
    seq_t phi_sigma = phi((idx_t)sigma_n);

    return gcd(phi_n, phi_sigma);
}

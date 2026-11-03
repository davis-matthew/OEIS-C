/*
 * A353643  The greatest common divisor of phi(n) and phi(sigma(n)).
 * https://oeis.org/A353643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(phi(n), phi(sigma(n))) // matches the OEIS data
 * Range:     u8 n <= 469, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 114 terms through n = 8234 (2026-11-01)
 * Helpers:   gcd, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/totient.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n) {
    /* φ(n) */
    seq_t phi_n = totient(n);

    /* σ(n) */
    seq_t sigma_n = sigma_of(n);

    /* φ(σ(n)) – σ(n) fits in idx_t for the range of n we use.
       Cast safely because σ(n) < 2^64 for n up to 10000.   */
    seq_t phi_sigma = totient((idx_t)sigma_n);

    return gcd(phi_n, phi_sigma);
}

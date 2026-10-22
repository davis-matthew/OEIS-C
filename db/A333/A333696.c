/*
 * A333696  Denominators of coefficients in expansion of Sum_{k>=1} phi(k) *
 *          log(1/(1 - x^k)).
 * https://oeis.org/A333696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of Sum_{k=1..n} 1/gcd(n,k) Equivalently a(n)
 *            = n / gcd( n, Σ_{d|n} d*φ(d) ) (φ = Euler totient)
 * Range:     u8 n <= 16, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   gcd, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* Σ_{d|n} d*φ(d) */
    seq_t sum = 0;
    idx_t d;
    for (d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            /* divisor d */
            seq_t d_seq = (seq_t)d;
            seq_t phi_d = phi(d);          /* phi() from utils/phi.h */
            sum += d_seq * phi_d;

            /* complementary divisor m = n/d, if distinct */
            idx_t m = n / d;
            if (m != d) {
                seq_t m_seq = (seq_t)m;
                seq_t phi_m = phi(m);
                sum += m_seq * phi_m;
            }
        }
    }
    seq_t g = gcd(sum, (seq_t)n);        /* gcd of sum and n */
    seq_t denom = (seq_t)n / g;          /* the desired denominator */
    return denom;
}

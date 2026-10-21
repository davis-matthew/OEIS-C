/*
 * A282548  Expansion of phi_{12, 1}(x) where phi_{r, s}(x) = Sum_{n, m>0} m^r
 *          * n^s * x^{m*n}.
 * https://oeis.org/A282548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n * sigma_k(n, 11) where sigma_k(n, k) = Σ_{d|n} d^k
 *            (here k = 11)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 40, u128 n <= 1625,
 *            big any n
 * Helpers:   sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_k.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }
    seq_t s = sigma_k(n, 11);          /* Σ_{d|n} d^11 */
    return ((seq_t)n) * s;              /* n * Σ_{d|n} d^11 = Σ_{d|n} d^12 * (n/d) */
}

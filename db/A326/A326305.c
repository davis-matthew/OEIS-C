/*
 * A326305  Dirichlet g.f.: zeta(s-1) * (1 - 2^(-s)) / zeta(s).
 * https://oeis.org/A326305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = phi(n) if n odd, phi(n) - phi(n/2) if n even
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;          /* undefined term, return 0 for safety */
    }
    seq_t phi_n = phi(n);          /* φ(n) */
    if (n & 1ULL) {                /* odd n */
        return phi_n;
    }
    seq_t phi_half = phi(n >> 1);  /* φ(n/2) */
    return phi_n - phi_half;      /* φ(n) - φ(n/2) */
}

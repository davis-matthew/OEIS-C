/*
 * A358714  a(n) = phi(n)^3.
 * https://oeis.org/A358714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = phi(n)^3
 * Range:     u8 n <= 10, u16 n <= 42, u32 n <= 1626, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */
    seq_t t = phi(n);             /* Euler’s totient φ(n) */
    return t * t * t;             /* φ(n)^3 */
}

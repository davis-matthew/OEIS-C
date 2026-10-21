/*
 * A299822  Product of Euler's totient and the squarefree kernel, a(n) =
 *          phi(n)*rad(n).
 * https://oeis.org/A299822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = phi(n) * rad(n)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi, rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/rad.h"

seq_t a(idx_t n)
{
    /* Return the product of Euler's totient and the square‑free kernel. */
    return phi(n) * rad(n);
}

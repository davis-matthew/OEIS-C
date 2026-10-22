/*
 * A334660  Dirichlet g.f.: 1 / zeta(s-4).
 * https://oeis.org/A334660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = mu(n) * n^4, where mu is the Möbius function
 * Range:     i8 n <= 4, i16 n <= 13, i32 n <= 216, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    /* Möbius function μ(n) */
    seq_t mu = mobius(n);

    /* Compute n^4 in seq_t */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;   /* n^2 */
    seq_t n4 = n2 * n2;   /* n^4 */

    return mu * n4;
}

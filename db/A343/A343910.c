/*
 * A343910  a(n) = mu(phi(n)), where mu is the Möbius function and phi is the
 *          Euler totient function.
 * https://oeis.org/A343910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = mu(phi(n))
 * Range:     i8 n <= 1253, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   mobius, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    seq_t ph = phi(n);
    return mobius((idx_t)ph);
}

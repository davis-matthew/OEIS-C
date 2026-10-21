/*
 * A293516  a(n) = phi(n) - 2*phi(phi(n)), where phi = Euler totient function,
 *          A000010.
 * https://oeis.org/A293516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = phi(n) - 2*phi(phi(n))
 * Range:     i8 n <= 329, i16 n <= 9814, i32 n <= 9814, i64 n <= 9814,
 *            i128 n <= 9814, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    seq_t phi_n = phi(n);
    seq_t phi_phi_n = phi((idx_t)phi_n);
    return phi_n - (seq_t)2 * phi_phi_n;
}

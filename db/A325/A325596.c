/*
 * A325596  a(n) = Sum_{d|n} mu(n/d) * (-1)^(d + 1) * d.
 * https://oeis.org/A325596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = phi(n) if n odd, phi(n) - 4*phi(n/2) if n even * (where
 *            phi = Euler's totient function from utils/phi.h)
 * Range:     i8 n <= 93, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* n is unsigned, n >= 1 by specification */
    if (n & 1UL)  /* odd */
        return phi(n);
    else          /* even */
    {
        seq_t phi_n   = phi(n);
        seq_t phi_half = phi(n >> 1);   /* n/2 */
        return phi_n - ((seq_t)4 * phi_half);
    }
}

/*
 * A292273  For odd n: a(n) = 0, and for even n: a(n) = -mu(n), where mu is
 *          Moebius function (A008683).
 * https://oeis.org/A292273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 0 if n is odd; a(n) = -mobius(n) if n is even
 * Range:     i8 n <= 9206, i16 n <= 9206, i32 n <= 9206, i64 n <= 9206,
 *            i128 n <= 9206, big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    if (n & 1u)
        return 0;
    return -mobius(n);
}

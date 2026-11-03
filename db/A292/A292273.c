/*
 * A292273  For odd n: a(n) = 0, and for even n: a(n) = -mu(n), where mu is
 *          Moebius function (A008683).
 * https://oeis.org/A292273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 0 if n is odd; a(n) = -mobius(n) if n is even
 * Range:     i8 n <= 65537, i16 n <= 65537, i32 n <= 65537, i64 n <= 65537,
 *            i128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9206 (2026-11-01)
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    if (n & 1u)
        return 0;
    return -mobius(n);
}

/*
 * A289641  a(n) = bigomega(n) - mu(n).
 * https://oeis.org/A289641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = bigomega(n) - mu(n)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   bigomega, mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"
#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    return bigomega(n) - mobius(n);
}

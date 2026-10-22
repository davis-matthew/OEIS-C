/*
 * A359484  a(n) = n * mu(n) if n is odd, otherwise n * mu(n) - (n/2) *
 *          mu(n/2).
 * https://oeis.org/A359484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = n * mu(n) if n is odd, else a(n) = n * mu(n) - (n/2) *
 *            mu(n/2)
 * Range:     i8 n <= 85, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    seq_t term = (seq_t)n * mobius(n);
    if ((n & 1) == 0) {                    /* even n */
        term -= (seq_t)(n >> 1) * mobius(n >> 1);
    }
    return term;
}

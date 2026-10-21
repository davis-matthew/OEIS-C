/*
 * A262804  a(n) = 2*b(n), where b(n) is defined by the condition that
 *          Product_{d|n} (b(d) + 1) = 1, n > 1 and b(1) = 1.
 * https://oeis.org/A262804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = moebius(n)*(moebius(n)+3)/2
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    /* Compute Möbius function μ(n) */
    seq_t m = mobius(n);
    /* a(n) = μ(n)*(μ(n)+3)/2 */
    return (m * (m + 3)) / 2;
}

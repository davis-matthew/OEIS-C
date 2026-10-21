/*
 * A256185  Second of two variations by Per Nørgård of his "infinity
 *          sequence", cf. A004718: v(0) = 0; v(3*n) = -v(n); v(3*n+1) = v(n)
 *          - 3; v(3*n+2) = -2 - v(n).
 * https://oeis.org/A256185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   calc_prime_noergard
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/calc_prime_noergard.h"

seq_t a(idx_t n)
{
    return calc_prime_noergard(n);
}

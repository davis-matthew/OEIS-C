/*
 * A293233  a(1) = 1; and for n > 1, a(n) = mu(n) * a(floor(n/2)), where mu is
 *          the Moebius function A008683.
 * https://oeis.org/A293233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1; for n > 1, a(n) = mu(n) * a(floor(n/2))
 * Range:     i8 n <= 9443, i16 n <= 9443, i32 n <= 9443, i64 n <= 9443,
 *            i128 n <= 9443, big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mobius.h"

seq_t a(idx_t n) {
    if (n == 1ULL)
        return 1;
    seq_t mu = mobius(n);
    if (mu == 0)
        return 0;
    return mu * a(n >> 1);  /* floor(n/2) */
}

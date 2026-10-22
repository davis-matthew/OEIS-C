/*
 * A330323  a(n) = Moebius(n)*Moebius(n+1).
 * https://oeis.org/A330323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = mu(n) * mu(n+1)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    /* Compute μ(n) and μ(n+1) via the provided helper and
       multiply them. μ values are in { -1, 0, 1 }, so the
       product is guaranteed to stay within seq_t. */
    seq_t mu_n   = mobius(n);
    seq_t mu_np1 = mobius(n + 1);
    return mu_n * mu_np1;
}

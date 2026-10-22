/*
 * A330324  a(n) = Sum_{i=1..n} Moebius(i)*Moebius(i+1), where Moebius(n) =
 *          A008683(n).
 * https://oeis.org/A330324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{i=1..n} Moebius(i) * Moebius(i+1)
 * Range:     i8 n <= 7597, i16 n <= 7597, i32 n <= 7597, i64 n <= 7597,
 *            i128 n <= 7597, big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mobius.h"

/*
 * Computes A330324: a(n) = Σ_{i=1..n} μ(i)·μ(i+1)
 * where μ is the Möbius function supplied in mobius.h.
 *
 * The implementation is an O(n) loop that accumulates the product of
 * successive Möbius values.  It uses only integer arithmetic as
 * required and respects the given types seq_t and idx_t.
 */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t mi  = mobius(i);
        seq_t mi1 = mobius(i + 1);
        sum += mi * mi1;
    }
    return sum;
}

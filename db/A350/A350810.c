/*
 * A350810  a(n) = ceiling((n-R(n^2))^2/(n+R(n^2))), where R(n^2) is the digit
 *          reversal of n^2.
 * https://oeis.org/A350810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(n) = ceil((n-R(n^2))^2/(n+R(n^2)))
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 426, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   revdecimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/revdecimal.h"
/* where R(x) is the decimal digit reversal of x. */

seq_t a(idx_t n)
{
    /* Cast the index to seq_t for arithmetic. */
    seq_t sn = (seq_t)n;

    /* Compute n^2. */
    seq_t sq = sn * sn;

    /* Reverse the decimal digits of n^2. */
    seq_t rev = revdecimal(sq);

    /* Compute the absolute difference |n - rev|. */
    seq_t diff = (sn >= rev) ? (sn - rev) : (rev - sn);

    /* Square the difference. */
    seq_t num = diff * diff;

    /* Compute the denominator n + rev. */
    seq_t den = sn + rev;

    /* Ceiling division of num by den. */
    seq_t result = (num + den - 1) / den;

    return result;
}

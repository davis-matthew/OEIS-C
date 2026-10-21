/*
 * A273878  Numerator of (2*(n+1)!/(n+2)).
 * https://oeis.org/A273878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator(2*(n+1)!/(n+2))
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   _small_gcd_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_small_gcd_idx.h"

seq_t a(idx_t n)
{
    idx_t m = n + 1;            /* upper limit for factorial (n+1)! */
    idx_t d = n + 2;            /* denominator of the fraction */
    seq_t result = 1;

    /* Handle the extra factor 2 in the numerator */
    idx_t g = _small_gcd_idx(2, d);
    result *= (seq_t)(2 / g);
    d /= g;

    /* Multiply by each integer from 1 to m, cancelling common divisor
       with the remaining part of the denominator. */
    for (idx_t j = 1; j <= m; ++j) {
        g = _small_gcd_idx(j, d);
        if (g > 1) {
            result *= (seq_t)(j / g);
            d /= g;
        } else {
            result *= (seq_t)j;
        }
    }

    /* At this point all common factors have been cancelled; the
       remaining product is the desired numerator. */
    return result;
}

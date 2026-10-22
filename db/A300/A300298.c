/*
 * A300298  Numerators of r(n) := Sum_{k=0..n-1} 1/Product_{j=0..4} (k + j +
 *          1), for n >= 0, with r(0) = 0.
 * https://oeis.org/A300298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of * n*(50 + 35*n + 10*n^2 + n^3) / * (96*(1 +
 *            n)*(2 + n)*(n + 3)*(4 + n))
 * Range:     u8 none, u16 n <= 2, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* cast index to seq_t */
    seq_t tn = (seq_t)n;

    /* auxiliary powers and shifts */
    seq_t t1 = tn + 1;
    seq_t t2 = tn + 2;
    seq_t t3 = tn + 3;
    seq_t t4 = tn + 4;

    /* compute numerator = tn * (50 + 35*tn + 10*tn^2 + tn^3) */
    seq_t tn2 = tn * tn;               /* tn^2 */
    seq_t tn3 = tn2 * tn;              /* tn^3 */
    seq_t tmp  = 50 + (seq_t)35 * tn + (seq_t)10 * tn2 + tn3;
    seq_t num  = tn * tmp;

    /* compute denominator = 96 * t1 * t2 * t3 * t4 */
    seq_t den  = 96 * t1 * t2 * t3 * t4;

    /* reduce the fraction */
    seq_t g = gcd(num, den);

    return num / g;
}

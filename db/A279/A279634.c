/*
 * A279634  Coefficients in the expansion of 1/([r] + [2r]x + [3r]x^2 + ...);
 *          [ ] = floor, r = 3/2.
 * https://oeis.org/A279634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1; a(1)=-3; a(2)=5; a(3)=-9; * for n>=4, a(n) = -2*a(n-1);
 *            equivalently * a(n) = 9 * (-1)^(n-2) * 2^(n-3) for n>=4
 * Range:     i8 n <= 6, i16 n <= 14, i32 n <= 30, i64 n <= 62, i128 n <= 126,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-3;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)-9;

    /* For n >= 4: start with 9 and apply the sign factor */
    seq_t val = (seq_t)9;
    if (n & 1)  /* n odd => sign negative */
        val = -val;          /* now val = -9 for odd n, 9 for even n */

    /* Multiply by 2^(n-3) */
    idx_t k = n - 3;
    for (idx_t i = 0; i < k; ++i)
        val = seq_mul(val, (seq_t)2);

    return val;
}

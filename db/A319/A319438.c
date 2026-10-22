/*
 * A319438  a(n) = 1^2 - 3^4 + 5^6 - 7^8 + 9^10 - 11^12 + 13^14 - ... + (up to
 *          n).
 * https://oeis.org/A319438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n*(n mod 2)*(-1)^⌊n/2⌋ + Σ_{i=1..⌊n/2⌋} (2*i-1)^(2*i) *
 *            (-1)^(i-1) (OEIS A319438) (This implements the explicit formula
 *            given in the OEIS entry.)
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 9, i64 n <= 17, i128 n <= 27,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"   /* provides: seq_t big_pow(seq_t base, seq_t exp); */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t half = n / 2;   /* floor(n/2) */

    for (idx_t i = 1; i <= half; ++i) {
        seq_t base = (seq_t)(2 * i - 1);
        seq_t exp  = (seq_t)(2 * i);
        seq_t term = big_pow(base, exp);

        /* Sign alternates: + for odd i, - for even i */
        if ((i & 1) == 0)       /* i even */
            sum -= term;
        else                    /* i odd  */
            sum += term;
    }

    if ((n & 1) != 0) {      /* n is odd: add the final odd term */
        seq_t sign = ((half & 1) == 0) ? (seq_t)1 : (seq_t)-1;
        sum += (seq_t)n * sign;
    }

    return sum;
}

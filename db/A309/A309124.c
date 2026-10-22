/*
 * A309124  a(n) = n - 3 * floor(n/3) + 5 * floor(n/5) - 7 * floor(n/7) + ...
 * https://oeis.org/A309124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n - 3*floor(n/3)+5*floor(n/5)-7*floor(n/7)+
 * Range:     i8 n <= 89, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = (seq_t)n;
    int sign = -1;            /* starts with a negative term for divisor 3 */
    for (idx_t d = 3; d <= n; d += 2) {
        idx_t q = n / d;      /* floor(n/d) */
        if (q == 0)          /* further odd divisors give 0 */
            break;
        result += ((seq_t)sign) * ((seq_t)d) * ((seq_t)q);
        sign = -sign;        /* alternate sign */
    }
    return result;
}

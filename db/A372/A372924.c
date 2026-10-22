/*
 * A372924  a(n) = (sum_digits(n^3)-n)/3.
 * https://oeis.org/A372924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base, easy
 * Formula:   a(n) = (sum_digits(n^3)-n)/3
 * Range:     i8 n <= 5, i16 n <= 31, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* convert index to the sequence type */
    seq_t sn = (seq_t)n;

    /* compute n^3 */
    seq_t n2 = sn * sn;   /* n^2 */
    seq_t n3 = n2 * sn;   /* n^3 */

    /* sum of digits of n^3 */
    seq_t sum = digit_sum(n3);

    /* (sum - n) / 3 */
    seq_t diff = sum - sn;
    return diff / 3;
}

/*
 * A249121  a(n) = n - (sum of digits of n) - (product of digits of n).
 * https://oeis.org/A249121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base, look
 * Formula:   a(n) = n - (sum of digits of n) - (product of digits of n)
 * Range:     i8 n <= 138, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   digit_product, digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum.h"
#include "../utils/digit_product.h"

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return sn - digit_sum(sn) - digit_product(sn);
}

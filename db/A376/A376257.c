/*
 * A376257  a(n) = (n - c(n))*(-1)^c(n), where c(n) is the product of the
 *          digits of n (A007954).
 * https://oeis.org/A376257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base, look
 * Formula:   a(n) = (n - c(n))*(-1)^c(n) where c(n) = product of decimal
 *            digits of n
 * Range:     i8 n <= 129, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   digit_product_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_product_uint.h"

seq_t a(idx_t n)
{
    seq_t prod = digit_product_uint(n);          /* c(n) */
    long long prod_ll = (long long)prod;        /* fits in signed 64-bit */
    int sign = (prod_ll & 1) ? -1 : 1;           /* (-1)^prod */
    seq_t result = ((seq_t)n - prod) * (seq_t)sign;
    return result;
}

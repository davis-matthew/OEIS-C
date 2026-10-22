/*
 * A302801  a(n) is the number whose digits result from the concatenation of
 *          the digits of n and the digits of the product of the digits of n.
 * https://oeis.org/A302801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concatenation of the decimal digits of n and of the
 *            product of its digits
 * Range:     u8 n <= 24, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   digit_len, digit_product, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_product.h"
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    seq_t prod = digit_product(ns);
    /* number of decimal digits of the product (0 has length 1) */
    int d = digit_len(prod);
    seq_t power = pow10((idx_t)d);
    return ns * power + prod;
}

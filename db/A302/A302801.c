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
 * Validated: partial, pass, 52 terms through n = 51 (2026-11-01)
 * Helpers:   digit_product, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/digit_product.h"
#include "../utils/num_digits.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    seq_t prod = digit_product(ns);
    /* number of decimal digits of the product (0 has length 1) */
    int d = num_digits(prod);
    seq_t power = pow10((idx_t)d);
    return ns * power + prod;
}

/*
 * A380873  Concatenate sum and product of decimal digits of n.
 * https://oeis.org/A380873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat(digit_sum(n), digit_product(n))
 * Range:     u8 n <= 24, u16 n <= 2788, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9403 (2026-11-01)
 * Helpers:   digit_product, digit_sum, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The concatenation is performed by appending the decimal representation
 * of the product to that of the sum.  The shift amount equals 10^d, where
 * d = digit_len(product).  For product = 0, digit_len returns 1. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/digit_product.h"
#include "../utils/digit_sum.h"
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
    /* Sum and product of decimal digits of n */
    seq_t sum  = digit_sum(n);
    seq_t prod = digit_product(n);

    /* Number of decimal digits of the product (≥1 even if prod=0) */
    idx_t  d    = (idx_t)num_digits(prod);

    /* Shift factor 10^d */
    seq_t shift = pow10(d);

    /* Concatenate: sum * 10^d + prod */
    return sum * shift + prod;
}

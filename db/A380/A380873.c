/*
 * A380873  Concatenate sum and product of decimal digits of n.
 * https://oeis.org/A380873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat(digit_sum(n), digit_product(n))
 * Range:     u8 n <= 24, u16 n <= 2788, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, digit_product, digit_sum, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The concatenation is performed by appending the decimal representation
 * of the product to that of the sum.  The shift amount equals 10^d, where
 * d = digit_len(product).  For product = 0, digit_len returns 1. */

#include "../utils/digit_sum.h"
#include "../utils/digit_product.h"
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Sum and product of decimal digits of n */
    seq_t sum  = digit_sum(n);
    seq_t prod = digit_product(n);

    /* Number of decimal digits of the product (≥1 even if prod=0) */
    idx_t  d    = (idx_t)digit_len(prod);

    /* Shift factor 10^d */
    seq_t shift = pow10(d);

    /* Concatenate: sum * 10^d + prod */
    return sum * shift + prod;
}

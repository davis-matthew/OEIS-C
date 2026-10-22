/*
 * A365762  Greatest common divisor of n and the product of its digits.
 * https://oeis.org/A365762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = gcd(n, product_of_digits(n))
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   digit_product, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/digit_product.h"

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    seq_t prod = digit_product(x);
    return gcd(x, prod);
}

/*
 * A277216  Product of decimal digits of sum of divisors of n.
 * https://oeis.org/A277216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = digit_product( sigma_of(n) )
 * Range:     u8 n <= 169, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_product, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* product of decimal digits of the sum of divisors of n */

#include "../utils/sigma_of.h"
#include "../utils/digit_product.h"

seq_t a(idx_t n)
{
    return digit_product(sigma_of(n));
}

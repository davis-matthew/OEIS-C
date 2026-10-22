/*
 * A316570  Multiplicative digital root of sigma(n).
 * https://oeis.org/A316570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A031347(sigma(n))
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   digit_product, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (i.e., multiplicative digital root of the sum of divisors of n) */

#include <stdint.h>
#include "../utils/sigma_of.h"
#include "../utils/digit_product.h"

seq_t a(idx_t n)
{
    seq_t s = sigma_of(n);
    seq_t r = s;
    while (r >= 10) {
        r = digit_product(r);
    }
    return r;
}

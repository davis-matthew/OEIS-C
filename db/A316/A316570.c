/*
 * A316570  Multiplicative digital root of sigma(n).
 * https://oeis.org/A316570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A031347(sigma(n))
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-01)
 * Helpers:   digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (i.e., multiplicative digital root of the sum of divisors of n) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_product.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t s = sigma_of(n);
    seq_t r = s;
    while (r >= 10) {
        r = digit_product(r);
    }
    return r;
}

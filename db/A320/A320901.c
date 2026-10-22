/*
 * A320901  Expansion of Sum_{k>=1} x^k/(1 + x^k)^4.
 * https://oeis.org/A320901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{d|n} (-1)^(d+1) * d*(d+1)*(d+2) / 6
 * Range:     i8 n <= 7, i16 n <= 55, i32 n <= 1810, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   term_div
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/term_div.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* Enumerate divisors in pairs (d, n/d) avoiding overflow in d*d */
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            sum += term_div(d);
            idx_t d2 = n / d;
            if (d2 != d)
                sum += term_div(d2);
        }
    }
    return sum;
}

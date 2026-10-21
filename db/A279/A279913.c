/*
 * A279913  Largest n-digit number ending in n.
 * https://oeis.org/A279913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n + 10^n - 10^(digits(n))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   digit_count, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/digit_count.h"

seq_t a(idx_t n)
{
    seq_t ten_pow_n = pow10(n);                 /* 10^n  */
    idx_t d = (idx_t)digit_count(n);            /* number of decimal digits of n */
    seq_t ten_pow_d = pow10(d);                 /* 10^digits(n) */
    return ten_pow_n + (seq_t)n - ten_pow_d;    /* a(n) = n + 10^n - 10^digits(n) */
}

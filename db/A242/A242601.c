/*
 * A242601  Integers repeated twice in a canonical order.
 * https://oeis.org/A242601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = floor((n+2)/4) * (-1)^floor((n+2)/2)
 * Range:     i8 n <= 81, i16 n <= 81, i32 n <= 81, i64 n <= 81, i128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Compute m = floor((n+2)/4) without 128-bit arithmetic */
    idx_t rem4 = n % 4;
    seq_t m = (seq_t)(n / 4 + (rem4 + 2) / 4);

    /* Compute e = floor((n+2)/2) */
    idx_t rem2 = n % 2;
    idx_t e = n / 2 + (rem2 + 2) / 2;

    /* Determine sign */
    seq_t sign = (e & 1) ? -1 : 1;

    return m * sign;
}

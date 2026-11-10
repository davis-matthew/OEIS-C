/*
 * A376356  Positions of numbers in A000462 that end in 2.
 * https://oeis.org/A376356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th m >= 1 such that A000462(m) ends in digit 2, i.e.
 *            the units digit of m in the triangular-number base is 2 (found
 *            by search)
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned A376356_last_digit(idx_t m) {
    idx_t r = m;
    idx_t k = 1;
    while ((k + 1) * (k + 2) / 2 <= r) k++;
    for (idx_t i = k; i >= 2; i--) {
        r %= i * (i + 1) / 2;
    }
    return (unsigned)r;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t m = 0;
    while (count < n) {
        m++;
        if (A376356_last_digit(m) == 2) count++;
    }
    return (seq_t)m;
}

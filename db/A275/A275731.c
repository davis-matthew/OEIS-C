/*
 * A275731  Transpose of array A275730.
 * https://oeis.org/A275731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A275730 read with row and column swapped (transpose);
 *            A(m,d) = m with its factorial-base digit at position d set to 0,
 *            read by antidiagonals
 * Range:     u8 n <= 1829, u16 n <= 1829, u32 n <= 1829, u64 n <= 1829,
 *            u128 n <= 1829, big any n
 * Validated: standard, pass, 136 terms through n = 1648 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A275731_zero_digit(idx_t m, idx_t d)
{
    idx_t val = 0, mult = 1, k = 2, i = 0;
    while (m > 0) {
        idx_t dig = m % k;
        m /= k;
        if (i != d) val += dig * mult;
        mult *= k;
        ++k;
        ++i;
    }
    return (seq_t)val;
}

seq_t a(idx_t n)
{
    idx_t t = 0;
    while ((t + 1) * (t + 2) / 2 <= n) ++t;
    idx_t off = n - t * (t + 1) / 2;
    idx_t row = off;
    idx_t col = t - off;
    return A275731_zero_digit(col, row);
}

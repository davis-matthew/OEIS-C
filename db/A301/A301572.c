/*
 * A301572  a(n) = distance from n to nearest Fibbinary number (A003714).
 * https://oeis.org/A301572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = minimum distance from n to a Fibbinary number (no
 *            consecutive 1s in binary)
 * Range:     u8 n <= 453, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 8957 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline bool is_fibbinary(seq_t x)
{
    return ((x & (x >> 1)) == 0);
}

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;

    /* distance to the nearest Fibbinary number below or equal to x */
    seq_t d_down = x;          /* maximum possible distance */
    seq_t y = x;
    while (1) {
        if (is_fibbinary(y)) {
            d_down = x - y;
            break;
        }
        if (y == 0)         /* reached zero */
            break;
        y--;
    }

    /* distance to the nearest Fibbinary number above or equal to x */
    seq_t d_up = 0;
    seq_t z = x;
    while (1) {
        if (is_fibbinary(z)) {
            d_up = z - x;
            break;
        }
        z++;
    }

    return (d_down < d_up) ? d_down : d_up;
}

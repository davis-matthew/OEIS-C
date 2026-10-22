/*
 * A389934  Number n in the rational base 7/3.
 * https://oeis.org/A389934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the decimal integer obtained by writing the rational
 *            base 7/3 * representation of n (digits 0..6) in usual decimal
 *            order. * The representation is computed by the recurrence * d =
 *            (3·N) mod 7 * N' = (3·N – d)/7 * iterated until N becomes 0,
 *            collecting the digits d from least to most * sign...
 * Range:     u8 n <= 6, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t m = (seq_t)n;
    uint8_t digs[64];
    size_t count = 0;

    while (m != (seq_t)0) {
        seq_t tmp = m * (seq_t)3;
        seq_t d   = tmp % (seq_t)7;          /* 0 <= d < 7 */
        digs[count++] = (uint8_t)d;
        m = (tmp - d) / (seq_t)7;
    }

    seq_t result = (seq_t)0;
    for (size_t i = 0; i < count; ++i) {
        result = result * (seq_t)10 + (seq_t)digs[count - 1 - i];
    }

    return result;
}

/*
 * A278169  Characteristic function for A000960.
 * https://oeis.org/A278169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = A100617(n) - A100617(n-1)
 * Range:     u8 n <= 9888, u16 n <= 9889, u32 n <= 9889, u64 n <= 9889,
 *            u128 n <= 9889, big any n
 * Validated: standard, pass, 125 terms through n = 9324 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A100617_a(idx_t n)
{
    seq_t m = (seq_t)n;
    seq_t k = 2;

    while (k <= m) {
        seq_t leave = m / k;
        if (!leave)
            break;
        m -= leave;
        ++k;
    }
    return m;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;
    return A100617_a(n) - A100617_a(n - 1);
}

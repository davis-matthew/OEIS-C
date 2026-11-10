/*
 * A382897  a(n) = n / A382895(n).
 * https://oeis.org/A382897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n / A382895(n), where A382895(n) divides n successively
 *            by its nonzero decimal digits (most to least significant),
 *            skipping digits that don't divide the current value exactly
 * Range:     u8 n <= 299, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 115 terms through n = 9895 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static seq_t A382895_a(idx_t n)
{
    seq_t val = (seq_t)n;
    if (val == 0) return (seq_t)0;

    uint8_t digits[70];
    size_t len = 0;
    seq_t tmp = val;
    while (tmp > 0)
    {
        digits[len++] = (uint8_t)(tmp % (seq_t)10);
        tmp /= (seq_t)10;
    }

    for (int i = (int)len - 1; i >= 0; --i)
    {
        uint8_t d = digits[i];
        if (d == 0) continue;
        if (val % (seq_t)d == 0)
            val /= (seq_t)d;
    }

    return val;
}

seq_t a(idx_t n)
{
    seq_t q = A382895_a(n);
    return (seq_t)n / q;
}

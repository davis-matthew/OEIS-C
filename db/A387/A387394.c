/*
 * A387394  Total number of 2's in the decimal digits of the divisors of n.
 * https://oeis.org/A387394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = total number of digit '2' in the decimal representations
 *            of all divisors of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 115 terms through n = 9483 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t count_2s(idx_t x)
{
    seq_t c = 0;
    while (x > 0) {
        if (x % 10 == 2) ++c;
        x /= 10;
    }
    return c;
}

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            result += count_2s(d);
            idx_t q = n / d;
            if (q != d) result += count_2s(q);
        }
    }
    return result;
}

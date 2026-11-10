/*
 * A331166  a(n) = min(n, A057889(n)), where A057889 is bijective base-2
 *          reverse.
 * https://oeis.org/A331166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = min(n, A057889(n)), where A057889(n) = reverse binary
 *            digits of the odd part of n, times the power of 2 dividing n
 * Range:     u8 n <= 255, u16 n <= 16383, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 115 terms through n = 9786 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A057889_a(idx_t n)
{
    if (n == 0) return 0;

    unsigned tz = 0;
    idx_t m = n;
    while ((m & 1) == 0) {
        m >>= 1;
        ++tz;
    }

    seq_t rev = 0;
    while (m) {
        rev = (rev << 1) | (seq_t)(m & 1);
        m >>= 1;
    }
    return rev << tz;
}

seq_t a(idx_t n)
{
    seq_t r = A057889_a(n);
    seq_t s = (seq_t)n;
    return (s < r) ? s : r;
}

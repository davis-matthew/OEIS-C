/*
 * A389900  a(n) = A126760(A126760(n)).
 * https://oeis.org/A389900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A126760(A126760(n)); A126760: a(0)=0, a(2n)=a(n),
 *            a(3n)=a(n), a(6n+1)=2n+1, a(6n+5)=2n+2
 * Range:     u8 n <= 2298, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 120 terms through n = 9351 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A126760_a(idx_t n)
{
    idx_t m = n;
    while (m > 0) {
        idx_t r = m % 6;
        if (r == 1) return (m - 1) / 6 * 2 + 1;
        if (r == 5) return (m - 5) / 6 * 2 + 2;
        if ((m & 1) == 0) { m >>= 1; continue; }
        if (m % 3 == 0) { m /= 3; continue; }
        break;
    }
    return 0;
}

seq_t a(idx_t n)
{
    return (seq_t)A126760_a(A126760_a(n));
}

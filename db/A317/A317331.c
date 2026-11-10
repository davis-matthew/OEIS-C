/*
 * A317331  Indices m for which A058304(m) = 1.
 * https://oeis.org/A317331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n + A014577(n-1) - 1, with A014577(0)=1,
 *            A014577(m)=A014577(m/2) for odd m, and for even m>0 it is 1 if
 *            m%4==0, else 0
 * Range:     u8 n <= 63, u16 n <= 16383, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 79 terms through n = 9919 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned A014577_a(idx_t n)
{
    if (n == 0)
        return 1;
    while (n != 0) {
        if (n & 1U) {
            n >>= 1;
        } else {
            return ((n & 3U) == 0U) ? 1U : 0U;
        }
    }
    return 1;
}

seq_t a(idx_t n)
{
    seq_t r = (seq_t)n * 4 - 1;
    if (A014577_a(n - 1))
        r = r + 1;
    return r;
}

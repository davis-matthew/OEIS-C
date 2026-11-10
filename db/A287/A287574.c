/*
 * A287574  Positions of 2 in A287571.
 * https://oeis.org/A287574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4n - A287571(n), where A287571(n) is computed from the
 *            base-4 digits of n-1 via the morphism 0->0312, 1->3120, 2->1203,
 *            3->2031
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9575 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned A287574_w(idx_t n)
{
    static const unsigned c[4] = {0, 3, 1, 2};
    idx_t m = n - 1;
    unsigned d = 0;
    int i;
    for (i = 31; i >= 0; --i) {
        unsigned k = (unsigned)((m >> (2 * i)) & 3);
        d = c[(k + d) & 3];
    }
    return d;
}

seq_t a(idx_t n)
{
    return (seq_t)n * 4 - (seq_t)A287574_w(n);
}

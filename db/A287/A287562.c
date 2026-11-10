/*
 * A287562  Positions of 0 in A287561.
 * https://oeis.org/A287562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(n-1) + 1 + ((4 - A287561(n)) mod 4), where A287561 is
 *            the fixed point of 0->0213, 1->2130, 2->1302, 3->3021
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9746 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t A287561_a(idx_t n)
{
    static const seq_t MAP[4][4] = {
        {0, 2, 1, 3},
        {2, 1, 3, 0},
        {1, 3, 0, 2},
        {3, 0, 2, 1}
    };
    if (n == 0)
        return 0;
    idx_t k = n - 1;
    unsigned char digits[64];
    size_t len = 0;
    while (k) {
        digits[len++] = (unsigned char)(k & 3);
        k >>= 2;
    }
    unsigned char d = 0;
    while (len--) {
        d = (unsigned char)MAP[d][digits[len]];
    }
    return (seq_t)d;
}

seq_t a(idx_t n)
{
    unsigned x = (unsigned)A287561_a(n);
    unsigned off = (4u - x) & 3u;
    return (seq_t)(n - 1) * 4 + 1 + (seq_t)off;
}

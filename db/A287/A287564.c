/*
 * A287564  Positions of 2 in A287561.
 * https://oeis.org/A287564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n - 3 + p(s), where s = A287561(n) and p(0)=1, p(1)=0,
 *            p(2)=3, p(3)=2 (the position of 2 in the image of s under
 *            0->0213, 1->2130, 2->1302, 3->3021)
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9888 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static unsigned A287564_s(idx_t n)
{
    static const unsigned char MAP[4][4] = {
        {0, 2, 1, 3},
        {2, 1, 3, 0},
        {1, 3, 0, 2},
        {3, 0, 2, 1}
    };
    idx_t k = n - 1;
    unsigned char digits[64];
    size_t len = 0;
    while (k) {
        digits[len++] = (unsigned char)(k & 3);
        k >>= 2;
    }
    unsigned char d = 0;
    while (len--) {
        d = MAP[d][digits[len]];
    }
    return d;
}

seq_t a(idx_t n)
{
    static const unsigned P[4] = {1, 0, 3, 2};
    unsigned s = A287564_s(n);
    return (seq_t)n * 4 - 3 + P[s];
}

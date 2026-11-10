/*
 * A287565  Positions of 3 in A287561.
 * https://oeis.org/A287565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4n - A287561(n), where A287561 is the fixed point of
 *            0->0213, 1->2130, 2->1302, 3->3021
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9216 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t A287561_a(idx_t n)
{
    static const unsigned char MAP[4][4] = {
        {0, 2, 1, 3},
        {2, 1, 3, 0},
        {1, 3, 0, 2},
        {3, 0, 2, 1}
    };
    if (n == 0) return 0;
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
    return (seq_t)d;
}

seq_t a(idx_t n)
{
    return (seq_t)n * 4 - A287561_a(n);
}

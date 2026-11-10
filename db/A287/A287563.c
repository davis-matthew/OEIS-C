/*
 * A287563  Positions of 1 in A287561.
 * https://oeis.org/A287563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(n-1) + j + 1, where j in {0..3} is the position of the
 *            1 in the morphism image (0->0213, 1->2130, 2->1302, 3->3021) of
 *            the letter A287561(n)
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9984 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static int A287563_letter(idx_t n)
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
    static const unsigned char MAP[4][4] = {
        {0, 2, 1, 3},
        {2, 1, 3, 0},
        {1, 3, 0, 2},
        {3, 0, 2, 1}
    };
    if (n == 0) return 0;
    int c = A287563_letter(n);
    int j = 0;
    while (MAP[c][j] != 1) j++;
    return (seq_t)(n - 1) * 4 + (seq_t)(j + 1);
}

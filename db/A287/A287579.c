/*
 * A287579  Positions of 2 in A287576.
 * https://oeis.org/A287579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(n-1) + 1 + p, where p is the 0-based position of 2 in
 *            the 4-block morph(A287576(n)); A287576 is the fixed point of
 *            0->0321, 1->3210, 2->2103, 3->1032
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9815 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint8_t A287579_morph[4][4] = {
    {0, 3, 2, 1},
    {3, 2, 1, 0},
    {2, 1, 0, 3},
    {1, 0, 3, 2}
};

static uint8_t A287579_sym(idx_t n)
{
    idx_t k = n - 1;
    uint8_t digits[40];
    int len = 0;
    while (k > 0) {
        digits[len++] = (uint8_t)(k & 3);
        k >>= 2;
    }
    uint8_t cur = 0;
    for (int i = len - 1; i >= 0; --i)
        cur = A287579_morph[cur][digits[i]];
    return cur;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    uint8_t w = A287579_sym(n);
    int p = 0;
    for (int j = 0; j < 4; ++j)
        if (A287579_morph[w][j] == 2)
            p = j;
    return (seq_t)4 * (seq_t)(n - 1) + (seq_t)(1 + p);
}

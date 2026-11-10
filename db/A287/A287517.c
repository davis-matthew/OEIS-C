/*
 * A287517  Positions of 0 in A287516.
 * https://oeis.org/A287517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n - 2 + [A287516(n) = 1], where A287516 is the fixed
 *            point of 0->012, 1->102, 2->021 (offset 1). Each block of 3
 *            holds one 0, at offset 0 for symbol 0 or 2 and offset 1 for
 *            symbol 1
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9726 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A287516_a(idx_t n)
{
    idx_t pos = n - 1;
    const int trans[3][3] = {
        {0, 1, 2},
        {1, 0, 2},
        {0, 2, 1}
    };
    idx_t tmp = pos;
    int digits[64];
    int len = 0;
    do {
        digits[len++] = (int)(tmp % 3);
        tmp /= 3;
    } while (tmp > 0);
    int cur = 0;
    for (int i = len - 1; i >= 0; --i)
        cur = trans[cur][digits[i]];
    return (seq_t)cur;
}

seq_t a(idx_t n)
{
    int s = (int)A287516_a(n);
    seq_t r = (seq_t)3 * (seq_t)(n - 1) + 1;
    if (s == 1) r = r + 1;
    return r;
}

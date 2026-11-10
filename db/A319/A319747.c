/*
 * A319747  a(n) = A319726(n) + A319657(n).
 * https://oeis.org/A319747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = A319726(n) + A319657(n): base-9 digits of n sorted
 *            decreasing, plus sorted increasing, both read in base 9
 * Range:     u8 n <= 83, u16 n <= 6560, u32 n <= 6560, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Validated: standard, pass, 80 terms through n = 6136 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    unsigned char cnt[9] = {0};
    idx_t t = n;
    while (t > 0) {
        cnt[(int)(t % 9)]++;
        t /= 9;
    }
    seq_t desc = 0, asc = 0;
    for (int d = 8; d >= 0; --d) {
        for (int k = 0; k < cnt[d]; ++k) {
            desc = desc * 9 + (seq_t)d;
        }
    }
    for (int d = 0; d < 9; ++d) {
        for (int k = 0; k < cnt[d]; ++k) {
            asc = asc * 9 + (seq_t)d;
        }
    }
    return desc + asc;
}

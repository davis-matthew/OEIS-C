/*
 * A297247  Up-variation of the base-16 digits of n; see Comments.
 * https://oeis.org/A297247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum over adjacent hex digits d[i],d[i+1] (most → least)
 *            where d[i+1] > d[i] of (d[i+1]-d[i]), i.e., the up‑variation of
 *            n in base 16
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert n to base‑16 digits (least significant first). */
    uint8_t digits[16];
    int len = 0;
    idx_t temp = n;

    if (temp == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (temp) {
            digits[len++] = (uint8_t)(temp & 0xF);
            temp >>= 4;
        }
    }

    seq_t sum = 0;
    int prev = digits[len - 1];
    for (int i = len - 2; i >= 0; --i) {
        int cur = digits[i];
        if (cur > prev) {
            sum += (seq_t)(cur - prev);
        }
        prev = cur;
    }

    return sum;
}

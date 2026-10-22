/*
 * A354719  Replace each even digit in n with the digit to its left.
 * https://oeis.org/A354719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, look, hear
 * Formula:   replace each even digit in n with the digit to its left
 *            (wrapping the leftmost digit to the rightmost)
 * Range:     u8 n <= 202, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    unsigned char digits[20];
    int len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        digits[len++] = tmp % 10;
        tmp /= 10;
    }

    unsigned char newrev[20];
    for (int i = 0; i < len; ++i) {
        unsigned char d = digits[i];
        if ((d & 1) == 0) {
            int left = (i == len - 1) ? 0 : i + 1;
            newrev[i] = digits[left];
        } else {
            newrev[i] = d;
        }
    }

    seq_t res = 0;
    for (int i = len - 1; i >= 0; --i) {
        res = res * 10 + (seq_t)newrev[i];
    }
    return res;
}

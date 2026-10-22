/*
 * A395316  a(n) is obtained by placing a '+' sign before each nonzero digit
 *          in the decimal expansion of n and evaluating the corresponding
 *          sum.
 * https://oeis.org/A395316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(n) = sum of the integers formed by starting at each * nonzero
 *            digit of the decimal representation of n and then appending *
 *            any following zeros until the next nonzero digit
 * Range:     u8 n <= 299, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    unsigned char digits[20];
    size_t len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        digits[len++] = tmp % 10;
        tmp /= 10;
    }
    seq_t sum = 0;
    seq_t cur = 0;
    for (size_t i = len; i > 0; --i) {
        unsigned d = digits[i-1];
        if (d != 0) {
            if (cur != 0) sum += cur;
            cur = (seq_t)d;
        } else {
            if (cur != 0) cur = cur * 10;
        }
    }
    if (cur != 0) sum += cur;
    return sum;
}

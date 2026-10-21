/*
 * A255594  Convert n to base 9, move least significant digit to most
 *          significant digit and convert back to base 10.
 * https://oeis.org/A255594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = rotation of the base‑9 representation of n, moving the
 *            least * significant digit to the most significant position
 * Range:     u8 n <= 84, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t m = n;
    unsigned char digit[32];
    int len = 0;

    if (m == 0) {
        digit[0] = 0;
        len = 1;
    } else {
        while (m > 0) {
            digit[len++] = (unsigned char)(m % 9);
            m /= 9;
        }
    }

    seq_t result = digit[0];
    for (int j = len - 1; j >= 1; --j) {
        result = result * 9 + digit[j];
    }
    return result;
}

/*
 * A243590  Numbers returned when each digit of n is replaced by the sum
 *          modulo 10 of the digits to its (wrapped) left and (wrapped) right.
 * https://oeis.org/A243590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   For a decimal integer n, replace each digit d(i) by (d(i-1) +
 *            d(i+1)) mod 10, treating indices cyclically. The function
 *            returns the resulting integer
 * Range:     u8 n <= 102, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Extract decimal digits of n into a temporary array.
       digits[0] holds the least‑significant digit. */
    unsigned char digits[64];
    unsigned char new_digits[64];
    int t = 0;
    idx_t m = n;

    /* At least one digit for n >= 1 */
    if (m == 0) {
        digits[t++] = 0;
    } else {
        while (m > 0) {
            digits[t++] = (unsigned char)(m % 10);
            m /= 10;
        }
    }

    /* Compute the new digits in the same reversed order. */
    for (int i = 0; i < t; ++i) {
        unsigned char left  = digits[(i + 1) % t];
        unsigned char right = digits[(i - 1 + t) % t];
        new_digits[i] = (unsigned char)((left + right) % 10);
    }

    /* Reconstruct the resulting number from most significant digit */
    seq_t result = 0;
    for (int i = t - 1; i >= 0; --i) {
        result = result * 10 + (seq_t)new_digits[i];
    }
    return result;
}

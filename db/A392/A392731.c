/*
 * A392731  Start at first leftmost digit of n. Move clockwise d steps, where
 *          d is the value of the active digit. Erase the digit landed upon.
 *          The new "active" digit is the one immediately following the one
 *          just erased. Repeat until only one digit remains. a(n) is the last
 *          remaining digit.
 * https://oeis.org/A392731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Simulates the elimination process for A392731:
   a(n) is the last remaining digit after repeatedly erasing the
   digit that is d steps clockwise from the current digit, where
   d is the value of the current digit. */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Convert n to a decimal digit array, most‑significant first. */
    unsigned char digits[64];   /* enough for up to ~39 digits of 128‑bit */
    size_t len = 0;
    seq_t temp = (seq_t)n;

    if (temp == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (temp > 0) {
            digits[len++] = (unsigned char)(temp % 10);
            temp /= 10;
        }
        /* reverse to left‑most first */
        for (size_t i = 0; i < len / 2; ++i) {
            unsigned char t = digits[i];
            digits[i] = digits[len - 1 - i];
            digits[len - 1 - i] = t;
        }
    }

    /* Simulation of the elimination process */
    size_t cur = 0;
    while (len > 1) {
        unsigned char d = digits[cur];
        size_t erase_idx = (cur + d) % len;

        /* Shift left to delete the element at erase_idx */
        for (size_t i = erase_idx; i + 1 < len; ++i)
            digits[i] = digits[i + 1];
        --len;

        /* New current is the element immediately after the erased one */
        cur = erase_idx % len;
    }

    return (seq_t)digits[0];
}

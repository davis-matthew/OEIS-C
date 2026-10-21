/*
 * A270172  Number of steps required to reach a single-digit number when
 *          successively increasing all digits of n by 1 simultaneously and
 *          removing leading zeros (9 becomes 0).
 * https://oeis.org/A270172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 267, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the number of steps required to reach a single‑digit
   number by successively increasing all digits of n by 1 modulo 10
   and removing leading zeros. */
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t val = (seq_t)n;

    /* Return 0 if n already has at most one digit. */
    if (val <= 9)
        return 0;

    /* Determine the number of decimal digits of val. */
    size_t len = 0;
    seq_t tmp = val;
    while (tmp != 0) {
        tmp /= 10;
        ++len;
    }

    /* Store the digits, most significant first. */
    unsigned char digits[100];   /* 100 is more than enough for 128‑bit integers */
    size_t i = len;
    tmp = val;
    while (tmp != 0) {
        digits[--i] = (unsigned char)(tmp % 10);
        tmp /= 10;
    }

    seq_t steps = 0;
    while (len > 1) {
        /* Increment every digit modulo 10. */
        for (size_t j = 0; j < len; ++j)
            digits[j] = (digits[j] + 1) % 10;

        /* Remove leading zeros that may appear after the increment. */
        while (len > 1 && digits[0] == 0) {
            for (size_t j = 0; j < len - 1; ++j)
                digits[j] = digits[j + 1];
            --len;
        }

        ++steps;
    }
    return steps;
}

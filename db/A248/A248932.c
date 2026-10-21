/*
 * A248932  Decimal expansion of 2^2203 - 1, the 16th Mersenne prime
 *          A000668(16).
 * https://oeis.org/A248932
 *
 * Offset:    664    (a(664) is the first term)
 * Keywords:  nonn, cons, easy, fini, full
 * Formula:   a(n) is the nth decimal digit of 2^2203–1, offset 664
 * Range:     u8 n <= 1327, u16 n <= 1327, u32 n <= 1327, u64 n <= 1327,
 *            u128 n <= 1327, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    const idx_t OFFSET = 664;          /* a(664) is the first digit */

    /* Static storage for the decimal digits of 2^2203–1.
       digits[0] holds the least‑significant digit. */
    static unsigned char digits[700];
    static size_t    len      = 0;    /* actual number of digits */
    static bool      init     = false;

    if (!init) {
        /* ------------------- compute 2^2203 ------------------- */
        /* start with value 1  */
        digits[0] = 1;
        len = 1;

        for (int k = 0; k < 2203; ++k) {     /* multiply by 2 2203 times */
            unsigned char carry = 0;
            for (size_t i = 0; i < len; ++i) {
                unsigned int  prod = digits[i] * 2 + carry;
                digits[i] = (unsigned char)(prod % 10);
                carry     = (unsigned char)(prod / 10);
            }
            if (carry > 0) {
                digits[len++] = carry;          /* carry is at most 1 */
            }
        }

        /* ------------------- subtract 1 ------------------- */
        size_t i = 0;
        while (true) {
            if (digits[i] > 0) {                /* simple case */
                digits[i]--;
                break;
            } else {                            /* propagate borrow */
                digits[i] = 9;
                ++i;
            }
        }

        init = true;
    }

    /* ---------------------------------------------------------------- */
    /* Return the desired digit, or 0 for positions outside the range. */
    if (n < OFFSET) return (seq_t)0;

    idx_t idx = n - OFFSET;                     /* 0‑based index from MSD */
    if ((size_t)idx >= len) return (seq_t)0;

    /* digits are stored least‑significant first:
       MS digit is at digits[len-1], so we map idx -> digits[len-1-idx] */
    return (seq_t)digits[len - 1 - idx];
}

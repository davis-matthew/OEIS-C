/*
 * A391205  a(n) is the least k for which n^k contains at least one decimal
 *          digit 7, or -1 if there is no such k.
 * https://oeis.org/A391205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   not stated
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n) = smallest k ≥ 1 such that n^k contains the digit 7,
   or -1 if no such k exists.  */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Limit the search to the conjectured maximum exponent 15.   */
    const idx_t MAX_K = 15;

    /* Treat n as a 64‑bit unsigned integer for the multiplication. */
    uint64_t base = (uint64_t)n;

    /* Allocate a dynamic array of decimal digits (little‑endian). */
    uint8_t *digits = (uint8_t *)malloc(sizeof(uint8_t) * 128);
    if (!digits) return (seq_t)-1;          /* allocation failure → -1 */

    size_t sz = 1;          /* initial value 1 */
    digits[0] = 1;

    for (idx_t k = 1; k <= MAX_K; ++k) {
        /* Multiply the current number by 'base'.  Use __int128 for safety. */
        unsigned __int128 carry = 0;
        for (size_t i = 0; i < sz; ++i) {
            unsigned __int128 prod = (unsigned __int128)digits[i] * base + carry;
            digits[i] = (uint8_t)(prod % 10);
            carry     = prod / 10;
        }
        while (carry > 0) {
            if (sz >= 128) {             /* simple growth; should never hit */
                digits = (uint8_t *)realloc(digits, sz + 128);
                if (!digits) return (seq_t)-1;
            }
            digits[sz++] = (uint8_t)(carry % 10);
            carry /= 10;
        }

        /* Scan for digit 7.  Since digits are stored little‑endian,
           any position is fine. */
        for (size_t i = 0; i < sz; ++i) {
            if (digits[i] == 7) {
                free(digits);
                return (seq_t)k;
            }
        }
    }

    free(digits);
    return (seq_t)-1;
}

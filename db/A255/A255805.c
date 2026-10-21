/*
 * A255805  Numbers with no zeros in base-8 representation.
 * https://oeis.org/A255805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 203, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Sequence: numbers whose base‑8 representation contains no digit 0 */
seq_t a(idx_t n)
{
    /* Find the number of octal digits of a(n). */
    idx_t cumulative = 0;
    idx_t pow7 = 7;          /* 7^len */
    size_t len = 1;

    while (cumulative + pow7 < n) {
        cumulative += pow7;
        pow7 *= 7;
        ++len;
    }

    /* Offset of a(n) within the block of numbers having 'len' octal digits.*/
    idx_t offset = n - cumulative - 1;   /* 0‑based index in the block */

    /* Convert this offset (in base 7) into an octal number whose digits
       are the offset's digits plus 1, then interpret that as a base‑8 integer. */
    seq_t result = 0;
    seq_t mult   = 1;   /* current power of 8 */

    for (size_t i = 0; i < len; ++i) {
        idx_t digit = offset % 7;           /* 0 … 6 */
        result += (seq_t)(digit + 1) * mult;
        mult *= 8;
        offset /= 7;
    }

    return result;
}

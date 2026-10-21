/*
 * A256303  Apply the transformation 0 -> 1 -> 2 -> 0 to the digits of n
 *          written in base 3; do not convert back to base 10.
 * https://oeis.org/A256303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = value obtained by writing n in base‑3, replacing each *
 *            digit d by (d+1) mod 3, and interpreting the resulting digits as
 *            a * decimal integer
 * Range:     u8 n <= 26, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* special case: n = 0 has base‑3 representation "0" → mapped to "1" */
    if (n == 0) return 1;

    /* buffer for base‑3 digits, LSB first */
    unsigned char digits[65];   /* 3^65 > 2^128, plenty for idx_t = 64‑bit */
    int dcount = 0;

    while (n > 0) {
        digits[dcount++] = n % 3;
        n /= 3;
    }

    seq_t result = 0;
    /* process digits from most significant to least to build the decimal number */
    for (int i = dcount - 1; i >= 0; --i) {
        unsigned char trit = digits[i];
        unsigned char newtrit = (trit + 1) % 3;
        result = result * 10 + newtrit;
    }
    return result;
}

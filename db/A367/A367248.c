/*
 * A367248  a(n) is the number of n-digit numbers whose difference between the
 *          largest and smallest digits is equal to 7.
 * https://oeis.org/A367248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 23*8^(n-1) - 41*7^(n-1) + 3*6^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                       /* undefined below offset, return 0 safely */

    idx_t i;
    /* compute 8^(n-1) and 7^(n-1) */
    seq_t pow8 = 1;
    seq_t pow7 = 1;
    for (i = 1; i < n; ++i) {
        pow8 *= (seq_t)8;
        pow7 *= (seq_t)7;
    }

    /* compute 6^n */
    seq_t pow6 = 1;
    for (i = 0; i < n; ++i)
        pow6 *= (seq_t)6;

    return (seq_t)23 * pow8 - (seq_t)41 * pow7 + (seq_t)3 * pow6;
}

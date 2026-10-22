/*
 * A397694  Residues of decimal digits mod 3, with 0 represented by 3.
 * https://oeis.org/A397694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number formed by replacing each decimal digit d of n by
 *            (d%3) if d%3!=0 else 3
 * Range:     u8 n <= 299, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)                         /* special case: a(0) = 3 */
        return (seq_t)3;

    /* Temporary buffer for the decimal digits (LSD first). */
    unsigned char digits[21];
    size_t len = 0;

    idx_t tmp = n;
    while (tmp != 0) {
        digits[len++] = tmp % 10;
        tmp /= 10;
    }

    seq_t res = 0;
    for (size_t i = len; i > 0; --i) {    /* process MSD first */
        unsigned d = digits[i - 1];
        unsigned m = d % 3;
        if (m == 0) m = 3;
        res = res * (seq_t)10 + (seq_t)m;
    }
    return res;
}

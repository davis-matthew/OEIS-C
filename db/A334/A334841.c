/*
 * A334841  a(0) = 0; for n > 0, a(n) = (number of 1's and 3's in base 4
 *          representation of n) - (number of 0's and 2's in base 4
 *          representation of n).
 * https://oeis.org/A334841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, base
 * Formula:   a(n) = (#digits equal to 1 or 3) - (#digits equal to 0 or 2) in
 *            base 4 representation of n, with a(0)=0
 * Range:     i8 n <= 88, i16 n <= 88, i32 n <= 88, i64 n <= 88, i128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = 0;
    idx_t x = n;
    while (x > 0) {
        uint8_t d = x & 3;          /* extract lowest base‑4 digit */
        if (d == 1 || d == 3) {
            res++;
        } else { /* d == 0 or d == 2 */
            res--;
        }
        x >>= 2;                    /* divide by 4 */
    }
    return res;
}

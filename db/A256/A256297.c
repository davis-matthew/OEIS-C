/*
 * A256297  Apply the transformation 0 -> 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 0 to
 *          the digits of n written in base 7, then convert back to base 10.
 * https://oeis.org/A256297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A256297: transform each base‑7 digit of n by (d+1)%7
   and interpret the resulting digits again as a base‑7 number. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;   /* base‑7 0 -> 1 */

    seq_t result = 0;
    seq_t power  = 1;
    idx_t temp   = n;

    while (temp > (idx_t)0) {
        idx_t digit      = temp % (idx_t)7;
        seq_t transformed = (seq_t)((digit + (idx_t)1) % (idx_t)7);
        result += transformed * power;
        power  *= (seq_t)7;
        temp   /= (idx_t)7;
    }
    return result;
}

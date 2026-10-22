/*
 * A374355  a(n) is the least fibbinary number f <= n such that n - f is also
 *          a fibbinary number whose binary expansion has no common 1's with
 *          that of f (where fibbinary numbers correspond to A003714).
 * https://oeis.org/A374355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is computed by removing every other bit of each run of
 *            consecutive 1's in the binary representation of n, starting with
 *            the leftmost bit of each run
 * Range:     u8 n <= 767, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t res = 0;
    /* find most significant bit position of n */
    idx_t msb = 0;
    idx_t tmp = n;
    while (tmp >> 1) {
        tmp >>= 1;
        msb++;
    }

    bool in_run = false;
    idx_t offset = 0;

    for (int i = (int)msb; i >= 0; --i) {
        if ((n >> i) & 1UL) {            /* current bit is 1 */
            if (!in_run) {               /* start of a run */
                in_run = true;
                offset = 0;               /* first bit of the run */
            } else {                     /* inside a run */
                offset++;
                if (offset & 1)           /* keep odd–offset bits */
                    res |= ((seq_t)1 << i);
            }
        } else {                         /* bit is 0 – end of a run */
            in_run = false;
        }
    }

    return res;
}

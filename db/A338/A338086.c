/*
 * A338086  Duplicate the ternary digits of n, so each 0, 1 or 2 becomes 00,
 *          11 or 22 respectively.
 * https://oeis.org/A338086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 4 * Sum_{i=0..k} d[i] * 9^i, where d[i] are the ternary
 *            digits of n
 * Range:     u8 n <= 8, u16 n <= 242, u32 n <= 6560, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t pow9  = 1;       /* 9^i for the current digit i */

    idx_t t = n;
    while (t) {
        unsigned int d = (unsigned int)(t % 3); /* d in {0,1,2} */
        seq_t digit   = (seq_t)(4 * d);          /* 0,4,8 in base‑9 */
        result += pow9 * digit;                 /* accumulate the term */
        pow9 *= 9;                              /* advance to next power of 9 */
        t /= 3;
    }
    return result;
}

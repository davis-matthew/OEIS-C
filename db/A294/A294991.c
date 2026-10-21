/*
 * A294991  Let S be the sequence of integer sets defined by the following
 *          rules: S(0) = {0}, S(1) = {1} and for any k > 0, S(2*k) = {2*k} U
 *          S(k) and S(2*k+1) = {2*k+1} U S(k) U S(k+1) (where X U Y denotes
 *          the union of the sets X and Y); a(n) = the number of elements of
 *          S(n).
 * https://oeis.org/A294991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*floor(log_2 n) - nu_2(n) + [n is a power of 2] + [first
 *            two bits of n in base 2 are 11] for n≥1; a(0) = 1 where nu_2(n)
 *            is the exponent of 2 in the factorisation of n. This
 *            implementation follows the stated formula and runs in O(1) time
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(0) is defined specially */
    if (n == 0)
        return 1;

    /* compute floor(log2(n)) as the index of the most significant
       set bit.  k will be in the range [0,63] for 64‑bit values. */
    unsigned int k = 0;
    idx_t tmp = n;
    while (tmp >>= 1)
        ++k;

    /* compute nu_2(n): number of trailing zero bits */
    unsigned int nu2 = 0;
    tmp = n;
    while ((tmp & 1ULL) == 0ULL) {
        ++nu2;
        tmp >>= 1;
    }

    /* start with 2*floor_log2 - nu_2 */
    seq_t result = (seq_t)(2 * k - nu2);

    /* add [n is a power of 2] */
    if ((n & (n - 1)) == 0ULL)
        ++result;

    /* add [first two bits of n in base 2 are 11] */
    if (k >= 1) {
        /* shift n right so that its two most significant bits
           occupy positions 1 and 0, then test if they are both 1. */
        if (((n >> (k - 1)) & 3ULL) == 3ULL)
            ++result;
    }

    return result;
}

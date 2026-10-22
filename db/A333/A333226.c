/*
 * A333226  Least common multiple of the n-th composition in standard order.
 * https://oeis.org/A333226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the LCM of the parts of the n-th composition in standard
   order, as defined for OEIS A333226.
*/

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Guard against the undefined case n == 0; the sequence is
       defined for n >= 1 only. */
    if (n == 0)
        return 0;

    seq_t lcm = 1;          /* current lcm value */
    idx_t prev_pos = 0;     /* previous position (starting with 0) */

    idx_t bit_index = 0;    /* position of the current bit (0-based) */
    idx_t v = n;            /* copy of n for manipulation */

    while (v)
    {
        if ((v & 1ULL) != 0)
        {
            idx_t pos = bit_index + 1;              /* 1‑based position of the bit */
            seq_t part = (seq_t)(pos - prev_pos);    /* composition part */

            seq_t g = gcd(lcm, part);                /* gcd */
            lcm = (lcm / g) * part;                  /* lcm update */

            prev_pos = pos;                          /* shift prev position */
        }
        v >>= 1;          /* move to the next bit */
        bit_index++;
    }

    return lcm;
}

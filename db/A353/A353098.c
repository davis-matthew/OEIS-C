/*
 * A353098  a(1) = 6; for n>1, a(n) = 7 * a(n-1) + 7 - n.
 * https://oeis.org/A353098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=6; a(n)=7*a(n-1)+7-n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) /* undefined, but guard */
        return 0;

    seq_t val = 6; /* a(1) */
    if (n == 1)
        return val;

    for (idx_t i = 2; i <= n; ++i)
    {
        val = 7 * val + 7 - (seq_t)i;
    }
    return val;
}

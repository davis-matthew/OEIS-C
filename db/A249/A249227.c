/*
 * A249227  Number of length n+5 0..2 arrays with no six consecutive terms
 *          having two times the sum of any two elements equal to the sum of
 *          the remaining four.
 * https://oeis.org/A249227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 486 * 2^(n-1) (offset = 1)
 * Range:     u8 none, u16 n <= 8, u32 n <= 24, u64 n <= 56, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)        /* values below the offset are undefined */
        return (seq_t)0;

    seq_t res = (seq_t)486;  /* start with a(1) */
    idx_t i;
    for (i = 1; i < n; ++i)
        res <<= 1;          /* multiply by 2 */

    return res;
}

/*
 * A255876  a(n) = (4*n^2 + 4*n - 3 - 3*(-1)^n)/2.
 * https://oeis.org/A255876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^2 + 4*n - 3 - 3*(-1)^n)/2
 * Range:     u8 n <= 10, u16 n <= 180, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert index to sequence type for exact arithmetic */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t val = n2 + n2;           /* 2*n^2 */
    val += nn + nn;                 /* +2*n */
    /* For even n subtract 3 */
    if ((n & 1ULL) == 0)
        val -= (seq_t)3;
    return val;
}

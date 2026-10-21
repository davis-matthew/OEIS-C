/*
 * A275496  a(n) = n^2*(2*n^2 + (-1)^n).
 * https://oeis.org/A275496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(2*n^2 + (-1)^n)
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 215, u64 n <= 9526,
 *            u128 n <= 9526, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* cast index to seq_t */
    seq_t n2 = nn * nn;           /* n^2 */
    seq_t n4 = n2 * n2;           /* n^4 */
    seq_t val = 2 * n4;           /* 2*n^4 */
    if (n & 1)                    /* odd n: subtract n^2 */
        val -= n2;
    else                          /* even n: add n^2 */
        val += n2;
    return val;
}

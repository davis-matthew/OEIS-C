/*
 * A258402  a(n) = (n^2 + 4*n + 6) * n^2.
 * https://oeis.org/A258402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + 4*n + 6) * n^2
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>           /* for uint64_t, etc. */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* convert index to seq_t */
    seq_t n2 = nn * nn;           /* n^2   */
    seq_t term = n2 + nn * (seq_t)4 + (seq_t)6;   /* n^2 + 4*n + 6 */
    return term * n2;              /* (n^2 + 4*n + 6) * n^2 */
}

/*
 * A272775  Squares of the form P(n, 5) + n, where P(x,k) is the Pochhammer
 *          function and n = square (A000290).
 * https://oeis.org/A272775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^5 + 5*n^3 + 5*n)^2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 9, u64 n <= 84, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn  = (seq_t)n;
    seq_t n2  = nn * nn;          /* n^2 */
    seq_t n3  = n2 * nn;          /* n^3 */
    seq_t n5  = n3 * n2;          /* n^5 = n^3 * n^2 */
    seq_t part = n5 + ((seq_t)5 * n3) + ((seq_t)5 * nn); /* n^5 + 5n^3 + 5n */
    return part * part;           /* square of the part */
}

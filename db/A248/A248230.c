/*
 * A248230  a(n) = floor(1/(zeta(4) - Sum_{h=1..n} 1/h^4)).
 * https://oeis.org/A248230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( (12*n^3 + 18*n^2 + 15*n + 4) / 4 ), equivalent to
 *            a(n) = floor(3*n^3 + 9*n^2/2 + 7*n/2 + n/4 + 1) with integer
 *            arithmetic
 * Range:     u8 n <= 2, u16 n <= 17, u32 n <= 729, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;           /* undefined for n < 1, return 0 */

    /* promote n to seq_t once */
    seq_t nn = (seq_t)n;

    /* compute n^2 and n^3 using seq_t arithmetic */
    seq_t n2 = nn * nn;        /* n^2 */
    seq_t n3 = n2 * nn;        /* n^3 */

    /* numerator = 12*n^3 + 18*n^2 + 15*n + 4 */
    seq_t numerator = (12 * n3) + (18 * n2) + (15 * nn) + (seq_t)4;

    /* integer division truncates towards zero -> floor for positive values */
    return numerator / (seq_t)4;
}

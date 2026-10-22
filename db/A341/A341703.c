/*
 * A341703  a(n) = 6*binomial(n,4) + 2*binomial(n,2) + 1.
 * https://oeis.org/A341703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4 - 10*n + 15*n^2 - 6*n^3 + n^4)/4
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 362, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* Convert index to seq_t */
    seq_t n2 = nn * nn;           /* n^2 */
    seq_t n3 = n2 * nn;           /* n^3 */
    seq_t n4 = n3 * nn;           /* n^4 */

    seq_t term = n4 - ((seq_t)6 * n3)
              + ((seq_t)15 * n2)
              - ((seq_t)10 * nn)
              + (seq_t)4;

    return term / ((seq_t)4);
}

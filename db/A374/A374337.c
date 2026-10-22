/*
 * A374337  Start with two vertices and draw a circle around each whose radius
 *          is the distance between the vertices. The sequence gives the
 *          number of regions constructed after n iterations of drawing
 *          circles with this same radius around every new vertex created from
 *          all circles' intersections.
 * https://oeis.org/A374337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A374337: Number of regions after successive circle drawings.
 * Formula implemented:  a(n) = (15*n^2 - 17*n + delta)/3
 *   delta = -7, if n mod 3 == 1 (except n=1, handled separately)
 *   delta = -3, if n mod 3 == 0
 *   delta = +7, if n mod 3 == 2
 *
 * The term for n = 1 is 3 (n mod 3 == 1 would give a negative numerator).
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)  /* offset is 1; behaviour below is irrelevant */
        return 0;

    if (n == 1)
        return 3;  /* special case: |(-3)| = 3 */

    seq_t nn = (seq_t)n;
    seq_t num;
    unsigned int mod = (unsigned int)(n % 3);

    /* Use 15*n^2 - 17*n + delta */
    if (mod == 0) {           /* n = 3*k */
        num = ((seq_t)15 * nn * nn)
              - ((seq_t)17 * nn)
              - ((seq_t)3);
    } else if (mod == 1) {    /* n = 3*k + 1, k >= 1 */
        num = ((seq_t)15 * nn * nn)
              - ((seq_t)17 * nn)
              - ((seq_t)7);
    } else {                  /* mod == 2, n = 3*k - 1, k >= 1 */
        num = ((seq_t)15 * nn * nn)
              - ((seq_t)17 * nn)
              + ((seq_t)7);
    }

    return num / (seq_t)3;
}

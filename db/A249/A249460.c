/*
 * A249460  Number of proper colorings of the cube with at most n colors under
 *          rotational symmetry.
 * https://oeis.org/A249460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-2)*(n-1)*n*(n^3-9*n^2+32*n-38)/24;
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n < 3 the formula yields 0 */
    if (n < 3)
        return 0;

    /* Cast n once for reuse */
    seq_t nn = (seq_t)n;
    seq_t nn2 = nn * nn;        /* n^2 */
    seq_t nn3 = nn2 * nn;       /* n^3 */

    /* n^3 - 9*n^2 + 32*n - 38 */
    seq_t expr = nn3
                 - (seq_t)9 * nn2
                 + (seq_t)32 * nn
                 - (seq_t)38;

    /* (n-2)*(n-1)*n*expr / 24 */
    seq_t result = (nn - (seq_t)2)
                   * (nn - (seq_t)1)
                   * nn
                   * expr;

    /* Divide by 24; the division is exact */
    result /= (seq_t)24;

    return result;
}

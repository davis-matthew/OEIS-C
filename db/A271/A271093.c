/*
 * A271093  Partial sums of the number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 275", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A271093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1. For n>0, if n is even: a(n) = (4*n^3 + 6*n^2 - 13*n +
 *            36)/6. If n is odd: a(n) = (4*n^3 + 18*n^2 + 11*n + 3)/6
 * Range:     u8 n <= 2, u16 n <= 24, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t nn = (seq_t)n;          /* convert index to seq_t */
    seq_t n2 = nn * nn;           /* n^2 */
    seq_t n3 = n2 * nn;           /* n^3 */
    seq_t numerator;

    if ((n & 1) == 0) {                   /* even n */
        numerator = 4 * n3
                   + 6 * n2
                   - 13 * nn
                   + 36;
    } else {                               /* odd n */
        numerator = 4 * n3
                   + 18 * n2
                   + 11 * nn
                   + 3;
    }

    return numerator / 6;
}

/*
 * A270008  Partial sums of the number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 5", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A270008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(n) = (4*n^3 + 6*n^2 - n) / 6 for n > 0 even a(n) =
 *            (4*n^3 + 18*n^2 + 23*n + 9) / 6 for n > 0 odd
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t nn = (seq_t)n;
    seq_t nn2 = nn * nn;           /* n^2 */
    seq_t nn3 = nn2 * nn;          /* n^3 */

    if ((n & 1) == 0) {            /* even n */
        return (4 * nn3 + 6 * nn2 - nn) / 6;
    } else {                       /* odd n */
        return (4 * nn3 + 18 * nn2 + 23 * nn + 9) / 6;
    }
}

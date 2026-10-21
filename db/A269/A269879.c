/*
 * A269879  First differences of number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 43", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A269879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*n^2 + 8*n + 4 for even n, -3*n^2 + 3 for odd n
 * Range:     i8 n <= 5, i16 n <= 103, i32 n <= 127, i64 n <= 127,
 *            i128 n <= 127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;          /* convert index to seq_t for arithmetic */
    if ((n & 1) == 0)             /* even n */
    {
        return sn * sn * 3 + sn * 8 + 4;
    }
    else                          /* odd n */
    {
        return -(sn * sn * 3) + 3;
    }
}

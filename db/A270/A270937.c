/*
 * A270937  First differences of number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 221", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A270937
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4-9*(-1)^n + (4+8*(-1)^n)*n + 4*(-1)^n*n^2 which
 *            simplifies to a(n) = 4n^2 + 12n - 5 for n even, n > 2 a(n) =
 *            -4n^2 - 4n + 13 for n odd, n > 2 a(0) = 7, a(1) = -4, a(2) = 40
 * Range:     i8 n <= 5, i16 n <= 89, i32 n <= 127, i64 n <= 127,
 *            i128 n <= 127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)7;
    if (n == 1) return (seq_t)-4;
    if (n == 2) return (seq_t)40;

    seq_t nn = (seq_t)n;
    if ((n & 1) == 0) {               /* n even */
        return (seq_t)4 * nn * nn + (seq_t)12 * nn - (seq_t)5;
    } else {                          /* n odd */
        return -( (seq_t)4 * nn * nn + (seq_t)4 * nn - (seq_t)13 );
    }
}

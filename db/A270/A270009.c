/*
 * A270009  First differences of number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 5", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A270009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 89, i32 n <= 127, i64 n <= 127,
 *            i128 n <= 127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0) = 7
 *          a(1) = -8
 *          for n >= 2:
 *          if n even: a(n) = 4*n^2 + 12*n + 9
 *          if n odd : a(n) = -4*n^2 - 4*n - 1
 */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 7;
    }
    if (n == 1) {
        return -8;
    }
    seq_t nn = (seq_t)n;
    if ((n & 1) == 0) {   /* even n >= 2 */
        return 4 * nn * nn + 12 * nn + 9;
    } else {              /* odd  n >= 3 */
        return -(4 * nn * nn + 4 * nn + 1);
    }
}

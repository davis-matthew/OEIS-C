/*
 * A269913  First differences of number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 3", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A269913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4*(1 + n + 2*(-1)^n*n + (-1)^n*n^2)
 * Range:     i8 n <= 5, i16 n <= 89, i32 n <= 127, i64 n <= 127,
 *            i128 n <= 127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* cast the index to seq_t so that only seq_t arithmetic is used */
    seq_t sn = (seq_t)n;
    /* (-1)^n: 1 if n is even, -1 if n is odd */
    seq_t sign = ((n & 1) ? -1 : 1);

    /* compute the inside expression: 1 + n + 2*sign*n + sign*n^2 */
    seq_t inside = 1 + sn + (sign * (2 * sn + sn * sn));

    /* final answer */
    return 4 * inside;
}

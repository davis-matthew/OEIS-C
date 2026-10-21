/*
 * A269909  First differences of number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 1", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A269909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (4-(-1)^n)+(4+8*(-1)^n)*n+4*(-1)^n*n^2 Equivalently: a(n)
 *            = 4*n^2+12*n+3 for n even a(n) = -4*n^2-4*n+5 for n odd
 * Range:     i8 n <= 5, i16 n <= 89, i32 n <= 127, i64 n <= 127,
 *            i128 n <= 127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    if ((n & 1) == 0) {                  /* even n */
        return (seq_t)4 * nn * nn + (seq_t)12 * nn + (seq_t)3;
    }                                     /* odd n */
    seq_t t = (seq_t)4 * nn * nn;
    return -t - (seq_t)4 * nn + (seq_t)5;
}

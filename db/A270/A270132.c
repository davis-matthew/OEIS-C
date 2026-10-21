/*
 * A270132  First differences of number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 89", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A270132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4 + 5*(-1)^n + (4 + 8*(-1)^n)*n + 4*(-1)^n*n^2 for n>3;
 *            Initial values: a(0)=3, a(1)=1, a(2)=39, a(3)=-44
 * Range:     i8 n <= 5, i16 n <= 89, i32 n <= 127, i64 n <= 127,
 *            i128 n <= 127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)39;
    if (n == 3) return (seq_t)-44;

    /* n > 3 */
    seq_t sign = (n & 1) ? -(seq_t)1 : (seq_t)1;
    seq_t nn   = (seq_t)n;
    seq_t res  = (seq_t)4 + (seq_t)5 * sign;
    res += (seq_t)(4 + 8 * sign) * nn;
    res += (seq_t)4 * sign * nn * nn;
    return res;
}

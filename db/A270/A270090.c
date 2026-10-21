/*
 * A270090  First differences of number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 73", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A270090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4 + 5*(-1)^n + (4 + 8*(-1)^n)*n + 4*(-1)^n*n^2 for n > 3
 *            with initial values: a(0)=3, a(1)=1, a(2)=35, a(3)=-40
 * Range:     i8 n <= 5, i16 n <= 89, i32 n <= 127, i64 n <= 127,
 *            i128 n <= 127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the first four terms directly */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)35;
    if (n == 3) return (seq_t)-40;

    /* (-1)^n : 1 if n is even, -1 if n is odd */
    seq_t parity = (n & 1) ? (seq_t)-1 : (seq_t)1;
    seq_t sn = (seq_t)n;

    /* Compute each part separately to avoid intermediate overflow */
    seq_t term1 = (seq_t)4;
    seq_t term2 = (seq_t)5 * parity;
    seq_t term3 = ((seq_t)4 + (seq_t)8 * parity) * sn;
    seq_t term4 = sn * sn;          /* n^2  (seq_t multiplication) */
    term4 *= (seq_t)4 * parity;    /* 4*(-1)^n*n^2 */

    return term1 + term2 + term3 + term4;
}

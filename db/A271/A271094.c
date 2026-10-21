/*
 * A271094  First differences of number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 275", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A271094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=4; a(1)=0; * For n>1: * if n even: a(n)=4*n^2+12*n-5 * if n
 *            odd : a(n)=-4*n^2-4*n+13
 * Range:     i8 n <= 5, i16 n <= 89, i32 n <= 127, i64 n <= 127,
 *            i128 n <= 127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)4;
    if (n == 1) return (seq_t)0;

    seq_t N = (seq_t)n;
    if ((N & 1) == 0)          /* even n */
        return (seq_t)(4 * N * N + 12 * N - 5);
    else                       /* odd n */
        return (seq_t)(-4 * N * N - 4 * N + 13);
}

/*
 * A271053  First differences of number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 253", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A271053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 7 a(1) = -4 a(2) = 40 for n > 2: if n is even → a(n) =
 *            4*n^2 + 12*n - 9 if n is odd → a(n) = -4*n^2 - 4*n + 17 This
 *            matches the OEIS description for A271053
 * Range:     i8 n <= 5, i16 n <= 89, i32 n <= 127, i64 n <= 127,
 *            i128 n <= 127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three values explicitly. */
    if (n == 0) return (seq_t)7;
    if (n == 1) return (seq_t)-4;
    if (n == 2) return (seq_t)40;

    /* From here n >= 3.  Perform the parity check and compute
       the quadratic formula using seq_t arithmetic. */
    if ((n & 1) == 0)     /* even n */
    {
        seq_t nn = (seq_t)n;
        return (seq_t)4 * nn * nn + (seq_t)12 * nn - (seq_t)9;
    }
    else                 /* odd n */
    {
        seq_t nn = (seq_t)n;
        return -(seq_t)4 * nn * nn - (seq_t)4 * nn + (seq_t)17;
    }
}

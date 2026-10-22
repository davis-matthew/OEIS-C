/*
 * A400232  a(n) is the number of boundary lattice points minus the number of
 *          interior lattice points of the cube [0,n] X [0,n] X [0,n].
 * https://oeis.org/A400232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -n^3 + 9*n^2 - 3*n + 3
 * Range:     i8 n <= 10, i16 n <= 35, i32 n <= 1293, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Convert the unsigned index to the arithmetic type used for the sequence */
    seq_t ns = (seq_t)n;
    seq_t n2 = ns * ns;          /* n^2 */
    seq_t n3 = n2 * ns;          /* n^3 */
    return -n3 + (seq_t)9 * n2 - (seq_t)3 * ns + (seq_t)3;
}

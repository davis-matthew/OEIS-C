/*
 * A263227  a(n) = n*(67*n - 89)/2.
 * https://oeis.org/A263227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n*(67*n - 89)/2
 * Range:     i8 n <= 2, i16 n <= 22, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Convert index to seq_t first to avoid mixing types */
    seq_t nn = (seq_t)n;
    /* Calculate the expression using only seq_t arithmetic */
    seq_t term = nn * ((seq_t)67 * nn - (seq_t)89);
    return term / (seq_t)2;
}

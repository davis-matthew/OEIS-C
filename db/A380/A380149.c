/*
 * A380149  Characteristic polynomial of the tesseract graph: a(n) =
 *          n^6*(n^2-16)*(n^2-4)^4.
 * https://oeis.org/A380149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n^6*(n^2-16)*(n^2-4)^4
 * Range:     i8 n <= 0, i16 n <= 2, i32 n <= 4, i64 n <= 15, i128 n <= 245,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t */
    seq_t n_seq = (seq_t)n;

    /* n^2 */
    seq_t n2 = n_seq * n_seq;

    /* n^6 = (n^2)^3 = n2 * n2 * n2 */
    seq_t n6 = n2 * n2 * n2;

    /* (n^2 - 16) */
    seq_t term1 = n2 - (seq_t)16;

    /* (n^2 - 4)^4 = (n^2 - 4) * (n^2 - 4) * (n^2 - 4) * (n^2 - 4) */
    seq_t n2m4 = n2 - (seq_t)4;
    seq_t term2 = n2m4 * n2m4 * n2m4 * n2m4;

    /* Final product */
    return n6 * term1 * term2;
}

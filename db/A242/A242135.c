/*
 * A242135  a(n) = n^3 - 2*n.
 * https://oeis.org/A242135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n^3 - 2*n
 * Range:     i8 n <= 5, i16 n <= 32, i32 n <= 1290, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    /* compute n^3 - 2*n, all in seq_t to avoid overflow of intermediate results */
    seq_t result = x * x * x - ((seq_t)2 * x);
    return result;
}

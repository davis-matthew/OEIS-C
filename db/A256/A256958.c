/*
 * A256958  The integers (shown from -50 on).
 * https://oeis.org/A256958
 *
 * Offset:    -50    (a(-50) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n
 * Range:     i8 n <= 50, i16 n <= 50, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)n;
}

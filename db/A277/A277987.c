/*
 * A277987  a(n) = 100*n - 28.
 * https://oeis.org/A277987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 100*n - 28
 * Range:     i8 n <= 1, i16 n <= 40, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute using the definition 100*n - 28.
       Cast n to seq_t first to avoid integer truncation. */
    return (seq_t)n * 100 - 28;
}

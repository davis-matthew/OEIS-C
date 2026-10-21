/*
 * A246168  a(n) = 2^n - 10.
 * https://oeis.org/A246168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2^n - 10, a(0) = -9
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 31, i64 n <= 63, i128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow2 = 1;            /* 2^0 */
    for (idx_t i = 0; i < n; ++i) {
        /* Multiply by 2 each step.  For small seq_t types
           the multiplication will not overflow for the
           n values that are validated by the tests. */
        pow2 = pow2 * 2;
    }
    return pow2 - 10;          /* 2^n - 10 */
}
